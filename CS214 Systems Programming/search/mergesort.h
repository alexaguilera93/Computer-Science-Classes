#ifndef MERGESORT_H
#define MERGESORT_H
void MergeSort(fileList** headRef);
fileList* SortedMerge(fileList* a, fileList* b);
void FrontBackSplit(fileList* source, fileList** frontRef, fileList** backRef);
void MergeSort(fileList** headRef);
#endif