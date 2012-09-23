/*
 *  This program explains POSIX Message Queue APIs
 *
 *  http://www.linuxpedia.org/index.php?title=Linux_System_Programming_:_POSIX_Message_Queue
 *
 *  Copyright (C) 2012  LinuxPedia.org (anoojgopi@linuxpedia.org)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <time.h>
#include <errno.h>
#include <unistd.h> /* Required for getpid function */

#define MQ_NAME "/mqlinuxpedia"
#define MQ_MESSAGE_MAX_LENGTH 100
#define MQ_MAX_NUM_OF_MESSAGES 10
#define MQ_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define MQ_RECEIVE_TIMEOUT 5001111 /* Message receive time out in nano seconds */

#define mq_printf(str, arg...) printf("%d : "str, getpid(), ##arg)

int main()
{
	mqd_t mqdes;
	struct mq_attr attr;
	int ret;
	char msg_buff[MQ_MESSAGE_MAX_LENGTH];
	ssize_t msg_len;
	unsigned msg_prio;
	struct timespec currenttime_before;
	struct timespec abs_timeout;
	struct timespec currenttime_after;

	/* Form the queue attributes */
	attr.mq_flags = 0; /* i.e mq_send will be block if message queue is full */
	attr.mq_maxmsg = MQ_MAX_NUM_OF_MESSAGES;
	attr.mq_msgsize = MQ_MESSAGE_MAX_LENGTH;
	attr.mq_curmsgs = 0; /* mq_curmsgs is dont care */

	/* Create message queue */
	mqdes = mq_open(MQ_NAME, O_RDONLY | O_CREAT, MQ_MODE, &attr);
	if( mqdes != (mqd_t)-1 )
	{
		mq_printf(" Message Queue Opened\n");
		mq_printf(" Message receive timeout %d sec, %d nanosec \n", 
				(MQ_RECEIVE_TIMEOUT / 1000000), (MQ_RECEIVE_TIMEOUT % 1000000));
		for(;;)
		{
			mq_printf(" Receiving message .... \n");

			/* get the current time at which mq_receive() is going to be called
			   i.e time since the Epoch time, 1970-01-01 00:00:00 +0000 (UTC) */
			clock_gettime( CLOCK_REALTIME, &currenttime_before);

			/* Calculate the time out value for message receive */
			abs_timeout.tv_sec = currenttime_before.tv_sec + (MQ_RECEIVE_TIMEOUT / 1000000);
			abs_timeout.tv_nsec = currenttime_before.tv_nsec + (MQ_RECEIVE_TIMEOUT % 1000000);

			/**********************************************************************************
			*
			*  #include <time.h>
			*  #include <mqueue.h>
			*  
			*  ssize_t mq_timedreceive(mqd_t mqdes, char *msg_ptr,
			* 	size_t msg_len, unsigned *msg_prio,
			* 	const struct timespec *abs_timeout);
			*
			*  mqdes       - Message queue descriptor.
			*  msg_ptr     - Address of the buffer to which the message to be stored.
			*  msg_len     - Size of the above buffer. 
			*                This value should be greater than the mq_msgsize attribute 
			*                           (see mq_getattr()) of the queue.
			*  msg_prio    - Non-NULL : Address of unsigned variable to which the priority 
			*                           of the received message to be stored.
			*                NULL     : Priority value is not returned.
			*  abs_timeout - Pointer to the timespec structure, which represents the 
			*                           absolute time (time since the Epoch time, 1970-01-01 
			*                           00:00:00 +0000 (UTC)) up to which this call will 
			*                           block if the message queue is empty.
			* 
			*                           struct timespec {
			*                              time_t tv_sec;  // Seconds since the Epoch time
			*                              long   tv_nsec; // Nanoseconds time fraction from 
			*                                              // above mentioned second value.
			*                           };
			* 
			*                           Normally the timespec structure value is formed by 
			*                           > Getting the current time using the function
			*                                  clock_gettime(CLOCK_REALTIME, &timespec_variable)
			*                           > then sdd the relative timeout value to timespec variable.
			* 
			*  Return value - On Success  : Number of bytes in the received message.
			*               - On failure : -1
			*
			***********************************************************************************/
			msg_len = mq_timedreceive (mqdes, msg_buff, MQ_MESSAGE_MAX_LENGTH, &msg_prio, &abs_timeout);

			/* Get the time at which the message queue returns */
			clock_gettime( CLOCK_REALTIME, &currenttime_after);

			/* Print all time values. We didn't print anything before to avoid the delay caused by printf() */
			mq_printf(" Time at which mq_timedreceive is called : %ld sec, %ld nanosec \n", 
						currenttime_before.tv_sec, currenttime_before.tv_nsec);
			mq_printf(" Timeout value passed to mq_timedreceive : %ld sec, %ld nanosec \n", 
						abs_timeout.tv_sec, abs_timeout.tv_nsec);
			mq_printf(" Time at which mq_timedreceive returned  : %ld sec, %ld nanosec \n", 
						currenttime_after.tv_sec, currenttime_after.tv_nsec);

			if(msg_len < 0)
			{
				/* mq_timedreceive returned with an error */
				if(errno == ETIMEDOUT)
				{
					mq_printf(" Message recieve timed out ! \n\n"); 
				}
				else
				{
					perror("   Failed");
					break;
				}
			}
			else
			{
				/* We got a message */
				mq_printf(" Successfully received %d bytes\n", (int)msg_len);

				/* Print the received message */
				/* Ensure the buffer is null terminated before printing it */
				msg_buff[MQ_MESSAGE_MAX_LENGTH-1] = '\0';
				mq_printf(" Message  : %s \n", msg_buff);
				mq_printf(" Priority : %d \n", msg_prio);
				break;
			}
		}
		
		/* Close the message queue */
		ret = mq_close(mqdes);
		if(ret)
			perror(" Message queue close failed");
		else
			mq_printf(" Message Queue Closed\n");
	
		ret = mq_unlink(MQ_NAME);
		if(ret)
			perror(" Message queue unlink failed");
		else
			mq_printf(" Message Queue unlinked\n");
	}
	else
	{
		perror(" Message queue open failed");
	}

	mq_printf("\n");
	return 0;
}
