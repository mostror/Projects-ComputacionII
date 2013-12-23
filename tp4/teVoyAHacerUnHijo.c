

	int count = sizeOfFile / 256;
	for(i = 0; i < count; i++)
	{
		sem_wait(sem);
		putNiceNameForBlingingProcessHere();
		sem_post(sem);
		mq_send(mqd, i, sizeof(int), 10);	
	}

	
