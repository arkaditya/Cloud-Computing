package Sort1;
/* author: Arkaditya 
 * package :  MySort 
 *  
 * * 
 */
public class MySort {
	
	static String[] array;
	int lIndex,rIndex;
	
	public MySort(String[] arr,int l, int r){
		array = arr;
		lIndex = l;
		rIndex = r;
	}
	
	/* Default Constructor to initialize null object in FileReadNew class*/
	public MySort () {	
	}
	
	public void QuickSort(String[] array,int lIndex,int rIndex) {
		
		int i = lIndex;
		int j = rIndex;
		int pivot;
				
		if ( j > i ) {			
			
			pivot = MySort.Partition(array, i, j);
			QuickSort(array,i,pivot-1);
			QuickSort(array,pivot+1,j);
			}
	}
	
	public static int Partition(String[] array,int lIndex, int rIndex) {
		
		int pivot = rIndex; 
		int l = lIndex - 1;
		for ( int r = lIndex; r <= rIndex - 1 ; r++ ) {
			
			if( array[r].compareTo(array[pivot]) < 0) {
				l++;
				MySort.Swap(array,l,r);			
			}
		}
		MySort.Swap(array,l+1,rIndex);
		return (l+1);
	}
		
	public static void Swap(String[] a ,int  lvalue, int rvalue) {
			
		String temp = a[lvalue];
		a[lvalue] = a[rvalue];
		a[rvalue] = temp;
	}
}