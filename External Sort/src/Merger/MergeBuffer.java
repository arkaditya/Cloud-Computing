package Merger;

/* author: Arkaditya 
 * package :  MergeBuffer  
 * * 
 */

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

public class MergeBuffer {
	
	String inputdata;
	public BufferedReader br;
	public File f;
	public static int BUFFER_SIZE = 4096;   
	//Input stream buffer fo 4096 bytes for each file
	
	public MergeBuffer(File filename) throws IOException {
		
		FileReader FReader = new FileReader(filename);
		br = new BufferedReader(FReader,BUFFER_SIZE);
		f = filename;
		readBuffer();
	}
	
	public void bufferClose() throws IOException {
		this.br.close();
	}
	
	public String popData() throws IOException {
		
		String data = getRow();
		readBuffer();
		return data;
	}
	
	public String getRow() throws IOException {
		
		if(empty()) {
			return null;
		} else {
		return inputdata.toString();
		}
	}
	
	public void readBuffer() throws IOException {
		
		this.inputdata = this.br.readLine();
	}
	
	public boolean empty() {
    	return this.inputdata == null;
    }
	
}	
