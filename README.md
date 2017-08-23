## Spherical K-means clustering

### Summary

This project contains a clustering algorithm based on spherical k-means for clustering objects corresponding to sparse high dimensional vectors. 



Dataset

The dataset is from the "Twenty Newsgroups Data Set" that is available at the UCI Machine Learning Repository


Preprocessing 

- Selects subset of the dataset - only the postings that correspond to original postings and not replies. 
- Cleaning - Eliminates non-ascii characters, removes case-sensitivity, tokenization
- Transforms into a Sparse Representation - (i) bag of words, and (ii) n-grams (n = 3, 5, 7).



Clustering

Input: 
- Vector-space representation of the objects
- number of clusters
- number of trials 
- class labels 

Output:
- Confusion matrix of dimensions (# of clusters)*(# of classes) whose entries will be the number of objects of a particular class that belongs to a particular cluster. 
- entropy and purity

```
sphkmeans input-file class-file #clusters #trials output-file
```

