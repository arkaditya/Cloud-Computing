package FileReadNew;

/* author: Arkaditya 
 * package :  FileReadNew 
 * * 
 */

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.PriorityQueue;

import Merger.MergeBuffer;
import Sort1.MySort;


public class FileReadNew 
{

	String inputFile;
	long bsize;
	MySort obj;

	FileReadNew(String filename, long blocksize, MySort SortObj) {

		inputFile = filename;
		bsize = blocksize;
		SortObj = new MySort();
	}

	FileReadNew() { // Constructor

	}

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub

		String filename;
		long blocksize;
		
		int threadCount = 1;
		try {
			if (args.length > 0) {				
				
				filename = args[0];
				blocksize = Long.parseLong(args[1]);
				ThreadCreation threadNo1 = new ThreadCreation(threadCount, filename, blocksize, null);
				Thread T1 = new Thread(threadNo1);
				T1.start();				
				//long timeTaken = endTime - startTime;
			}
			else {
				System.err.println("ERROR : INVALID NO OF ARGS");
				System.out.println("Enter arg[0] -> filename with absolute path "
									+  "arg[1] -> chunk size in bytes like 200000000 ");
				System.exit(1);
			}

		} catch (IllegalArgumentException err) {
			System.err.println("Command Line Argument exception...Exiting ");
			System.exit(1);
		}

	}

	public int mergerFiles(List<File> files, File output) throws IOException {

		// System.out.println("INSIDE MERGER..");
		PriorityQueue<MergeBuffer> queue;
		int InitialLength = 10;
		MergeBuffer MyBuffer;
		int count = 0;

		final Comparator<String> MyComp = new Comparator<String>() {

			@Override
			public int compare(String String1, String String2) {

				return String1.compareTo(String2);
			}
		};

		Comparator<MergeBuffer> MyBinary = new Comparator<MergeBuffer>() {

			public int compare(MergeBuffer i, MergeBuffer j) {

				int n = 0;
				try {
					n = MyComp.compare(i.getRow(), j.getRow());
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				return n;
			}
		};


		queue = new PriorityQueue<MergeBuffer>(InitialLength, MyBinary);

		for (File file : files) {

			MyBuffer = new MergeBuffer(file);
			queue.add(MyBuffer);
		}

		FileWriter f = new FileWriter(output);
		//System.out.println(output);

		BufferedWriter buffRdr = new BufferedWriter(f);

		try {
			while (queue.size() > 0) {

				MyBuffer = queue.poll();
				String row = MyBuffer.popData();

				buffRdr.write(row);
				buffRdr.newLine();
				count++;

				if (!MyBuffer.empty()) {
					queue.add(MyBuffer); // add it back
				} else {
					MyBuffer.br.close();
					MyBuffer.f.delete();
				}
			}
		} finally {

			buffRdr.close(); // Close the buffer reader

			for (MergeBuffer bufferB : queue) {
				bufferB.bufferClose();
			}
		}
		return count;
	}
}

class ThreadCreation implements Runnable {

	int threadCount;
	String filename;
	long chunkSize;
	long initPos;
	long finalPos;
	MySort obj;

	private volatile ArrayList<File> fileList = new ArrayList<File>();

	//File foutput = new File("D:\\tmp\\MergedOutput");
	File foutput = new File("/tmp/MergedOutput");

	ThreadCreation() {
	}

	ThreadCreation(int threads, String file, long blocksize, MySort S) {

		threadCount = threads;
		filename = file;
		chunkSize = blocksize;
		obj = S;
	}

	FileReadNew fr = new FileReadNew();

	public void run() {

		try {
			float ComputeTime;
			long startTime = System.currentTimeMillis();
			
			if (foutput.exists()) {
				foutput.delete();
				//System.out.println("MergedOutput deleted");
			}
			
			foutput.createNewFile();
			fileReader(filename, chunkSize, null);
			fileList = getFileList();
			fr.mergerFiles(fileList, foutput);
			long endTime = System.currentTimeMillis();
			ComputeTime  = (float)(endTime - startTime)/1000;
			System.out.println("Time Taken: " + ComputeTime +" seconds");

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void fileReader(String filename, long blocksize, MySort SortObj) throws IOException {

		String line;
		// BufferedReader r = Files.newBufferedReader(Paths.get(filename));
		File finput = new File(filename);
		
		if (finput.exists() && !finput.isDirectory()) {
			BufferedReader r = new BufferedReader(new FileReader(finput));

			long NumSplits = finput.length() / (blocksize * threadCount);
			long finalpos, position = 0L;
			long counter = blocksize / 100;
			
			//System.out.println("SPLITS IS " + NumSplits);			
			int buffer_len = (int) counter;

			for (long split = 0L; split < NumSplits; split++) {

				//String temp = "D:\\tmp\\" + split + ".temp";
				String temp = "/tmp/" + split + ".temp";

				finalpos = position;
				BufferedWriter w = Files.newBufferedWriter(Paths.get(temp));
				File f = new File(temp);

				int i = 0;

				ArrayList<String> dataList = new ArrayList<>(buffer_len);

				while ((finalpos - position) != counter && (line = r.readLine()) != null) {
					dataList.add(line);
					i++;
					finalpos++;
				}

				// SortObj = new MySort(a,0,a.length - 1);
				// SortObj.QuickSort(a, 0, a.length-1);
				Collections.sort(dataList);
			
				for (String s : dataList) {
					w.write(s);
					w.newLine();
				}

				// System.out.println(split + ".temp");
				fileList.add(f);

				w.flush();
				w.close(); // Close the temp file
				position = finalpos;
			}
			r.close();
			// return files;
		}
	}

	public ArrayList<File> getFileList() {

		return fileList;
	}

}