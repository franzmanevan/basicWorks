import java.util.*;

public class Solution {
	/**
	 * Method that creates a 2D array to contain two sorted lists
	 * in ascending and descending order. 
	 * @param list The list to be sorted.
	 * @return returns the 2D sorted array (ascending, descending).
	 */
	public static int[][] mergeSort(int[] list) { 
		// Declare two arrays with the list values. One will be used to sort
		// in ascending order, one for descending order.
		int[] ascend = new int[list.length];
		ascend = list;
		int[] descend = new int[list.length];
		descend = list;
		// Store values for the two arrays in a 2D array arr.
		int arr[][] = {mergeSrt(ascend, false), mergeSrt(descend, true)};
		return arr;
	}
	/**
	 * Recursive method that splits up an array into left and right
	 * to return a sorted method either ascending or descending.
	 * @param array array that is to be sorted.
	 * @param descend boolean to determine the order of the sort.
	 * @return returns the array sorted in the specified order.
	 */
	public static int[] mergeSrt(int[] array, boolean descend) {
		// Base case: if the array is only 1 element, it is considered
		// sorted.
		if (array.length <= 1) {
			return array;
		}
		// Variables to split the array in half at the middle into
		// left and right.
		int mid = array.length / 2;
		int[] left = new int[mid];
		int[] right;
		// Need to check if the array size is even or odd in order to not
		// miss a value.
		if (array.length % 2 == 0) {
			right = new int[mid];
		} else {
			right = new int[mid + 1];
		}
		// Fill arrays with appropriate values.
		for (int i = 0; i < left.length; i++) {
			left[i] = array[i];
		}
		for (int i = 0; i < right.length; i++) {
			// Needs to get the right half so it goes from mid +1 to the end.
			right[i] = array[mid + i];
		}
		// Recursively call the method to split until they are only length 1.
		left = mergeSrt(left, descend);
		right = mergeSrt(right, descend);
		// Use merge method to determine which elements of each array will go 
		//into the resulting array.
		int[] sorted = merge(left, right, descend);
		// Return the sorted values.
		return sorted;
	}
	/**
	 * Method to merge the left and right of the array in either ascending
	 * or descending order.
	 * @param left the left portion of the array.
	 * @param right the right portion of the array.
	 * @param descend boolean to determine the order of the sort.
	 * @return returns the two parts sorted in the specified order.
	 */
	public static int[] merge(int[] left, int[] right, boolean descend) {
		// Define array to store sorted values and return. Need counters
		// for the left, right and sorted array to iterate through.
		int[] array = new int[left.length + right.length];
		int l = 0, r = 0, a = 0;
		// While there are elements in either the left or right array,
		// iterate thru, sort, and store the appropriate values in array.
		while(l < left.length || r < right.length) {
			// Determines if there are values in both the right and left
			// arrays, just the left array, or just the right array.
			if (l < left.length && r < right.length) {
				if (descend) {  // Determines order (ascending vs descending).
					if (left[l] < right[r]) {
						array[a++] = right[r++];
					} else {
						array[a++] = left[l++];
					}					} else {
					if (left[l] < right[r]) {
						array[a++] = left[l++];
					} else {
						array[a++] = right[r++];
					}
				}
			} else if (l < left.length) { // Elements in just left array
				array[a++] = left[l++];
			} else if (r < right.length) { // Elements in just right array
				array[a++] = right[r++];
			}
		}
		// Returns the merged array.
		return array;
	}
	
	public static void main(String[] args) {
		int[] list = {12, 11, 13, 6, 5};
		int sorted[][] = mergeSort(list);
	}
}
/*
 * 
 */
/*
 * 
 */





