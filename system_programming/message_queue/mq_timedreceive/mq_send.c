/*
 *  This program explains POSIX Message Queue APIs
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
#include <unistd.h> /* Required for getpid function */
#include <string.h>

#define MQ_NAME "/mqlinuxpedia"
#define MQ_MESSAGE_DATA "hello world"
#define MQ_MESSAGE_LENGTH 12
#define MQ_MESSAGE_PRIORITY 0

#define mq_printf(str, arg...) printf("%d : "str, getpid(), ##arg)

int main (int argc, char ** argv)
{
	mqd_t mqd;
	int ret;
	char *message = NULL;
	int length;

	if(argc != 2)
	{
		mq_printf(" Please pass the message to send \n");
		return -1;
	}

	message = argv[1];
	length = strlen(message) + 1;
	
	/* Open the message queue. Message queue is already created from a different process */
	mqd = mq_open(MQ_NAME, O_WRONLY);
	if( mqd != (mqd_t)-1 )
	{
		mq_printf(" Message Queue Opened\n");

		mq_printf(" Sending message.... ");
		ret = mq_send(mqd, message, length, MQ_MESSAGE_PRIORITY);
		if(ret)
			perror("Failed");
		else
			mq_printf("Done\n");
		
		/* Close the message queue */
		ret = mq_close(mqd);
		if(ret)
			perror(" Message queue close failed");
		else
			mq_printf(" Message Queue Closed\n");
	
	}
	else
	{
		perror(" Message queue open failed ");
	}

	return 0;
}
