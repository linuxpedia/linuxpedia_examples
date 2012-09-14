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

#define MQ_NAME "/mqlinuxpedia"
#define MQ_MESSAGE_DATA "hello world"
#define MQ_MESSAGE_LENGTH 12
#define MQ_MESSAGE_PRIORITY 0

int main()
{
	mqd_t mqd;
	int ret;

	/* Open the message queue. Message queue is already created from a different process */
	mqd = mq_open(MQ_NAME, O_WRONLY);
	if( mqd != (mqd_t)-1 )
	{
		printf(" Message Queue Opened\n");

		printf(" Sending message.... ");
		ret = mq_send(mqd, MQ_MESSAGE_DATA, MQ_MESSAGE_LENGTH, MQ_MESSAGE_PRIORITY);
		if(ret)
			perror("Failed");
		else
			printf("Done\n");
		
		/* Close the message queue */
		ret = mq_close(mqd);
		if(ret)
			perror(" Message queue close failed");
		else
			printf(" Message Queue Closed\n");
	
	}
	else
	{
		perror(" Message queue open failed ");
	}

	return 0;
}
