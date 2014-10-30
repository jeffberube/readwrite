/*
 * @Author:	Jeff Berube
 * @Title:	ReadWrite (Java)
 * @Description:	Implements a solution to the reader writer problem with reader
 * 			bias using semaphores.
 *
 * @Usage:	java ReadWrite
 *
 */

import java.util.concurrent.Semaphore;

public class ReadWrite {

	/* Define class variables */
	private static final int NUM_READERS = 4;
	private static final int NUM_WRITERS = 1;

	private static final Semaphore x = new Semaphore(NUM_READERS, true);
	private static final Semaphore wsem = new Semaphore(NUM_WRITERS, true);
	private static int readcount = 0;

	private static Thread[] threads = new Thread[NUM_READERS + NUM_WRITERS];

	public static void main(String[] args) {

		System.out.println("ReadWrite started with " + NUM_READERS +
					" readers and " + NUM_WRITERS + " writers.");

		int i = 0;

		/* Create reader threads */
		while (i < NUM_READERS) {

			threads[i] = new Thread(new Reader(i + 1));
			threads[i].start();
	
			i++;

		}

		i = 0;

		/* Create writer threads */
		while (i < NUM_WRITERS) {
		
			threads[i + NUM_READERS] = new Thread(new Writer(i + 1));
			threads[i + NUM_READERS].start();
			
			i++;

		}


		/* Wait on threads to finish */
		i = 0;

		while (i < (NUM_READERS + NUM_WRITERS)) {
		
			try {

				threads[i++].join();
			
			} catch (InterruptedException ie) {}

		}

		/* One last little output for prettiness */
		System.out.println("\n\n");

	}	

	static class Reader implements Runnable {

		private int threadnum;

		public Reader(int threadnum) {

			this.threadnum = threadnum;

		}

		public void run() {

			while (true) {
		
				/* Sleep */
				try {			
				
					Thread.sleep(threadnum * 3000);
					x.acquire();


					/* Acquire semaphore and increase reader count */
					readcount++;

					/* If first reader, acquire writer semaphore */
					if (readcount == 1) wsem.acquire();

					/* Release reader count semaphore */
					x.release();
			
					/* Display thread status, sleep and 
					 * display status again */
					System.out.println("Reader " + threadnum
						+ ": Inside critical section. Reading.");

					Thread.sleep(threadnum * 1000);
				
					System.out.println("Reader " + threadnum
						+ ": Done reading. Exiting critical section.");

					/* Down on reader count semaphore and 
					 * update reader count */
					x.acquire();
					readcount--;

					/* If we were last reader, release writer semaphore */
					if (readcount == 0) wsem.release();

					/* Release reader count semaphore */
					x.release();
				
				} catch (InterruptedException e) {}

			}
		
		}

	}

	
	static class Writer implements Runnable {

		private int threadnum;

		public Writer(int threadnum) {

			this.threadnum = threadnum;

		}

		public void run() {

			while (true) {
				
				try {

					/* Sleep */
					Thread.sleep(threadnum * 3000);
			
					/* Acuire writer semaphore */
					wsem.acquire();

					/* Display status, sleep, update status */
					System.out.println("Writer " + threadnum
						+ ": Inside critical section. Writing.");

					Thread.sleep(threadnum * 1000);
			
					System.out.println("Writer " + threadnum
						+ ": Done writing. Exiting critical section.");

					/* Release writer semaphore */
					wsem.release();
			
				} catch (InterruptedException e) {}

			}

		}

	}

}


