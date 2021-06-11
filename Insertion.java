import java.util.*;

public class Insertion {
	public int[] insertionSort(int[] list,int N) { 
		// Used to determine if which iteration to print out
		int count = 1;
		// For loop that iterates through the sorted array
		for (int j = 1; j < list.length; j++) {
			// Used to keep the value we are trying to sort
			int key = list[j];
			// The values before it that are to be moved up
			int i = j - 1;
			// Loops thru all previous elements and move
			// past key value if they are larger, stops
			// when they are smaller or is first element
			while( i >= 0 && list[i] > key) {
				list[i + 1] = list[i];
				list[i] = key;
				i--;
				// Checks if this is the preferred iteration
				// If it is, return array, if not keep going
				if (count == N) {
					return list;
				} else {
					count++;
				}
			}
		}
		return list;
	}
}