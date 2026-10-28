Algorithm: Comparing Partitional (K-Means) and Hierarchical (Agglomerative) Clustering for Customer Segmentation on Mall Customers Data

Load Data:

Look for Mall_Customers.csv next to exp16.py. If found, load it and rename the columns to Annual_Income_k and Spending_Score.
Otherwise, print a warning and generate 200 simulated customers (random seed 42) with CustomerID, Age, Annual_Income_k (15-134) and Spending_Score (1-99).

Select & Scale Features:

Extract the two clustering features: Annual_Income_k and Spending_Score.
Standardize them with StandardScaler (zero mean, unit variance) so that both features carry equal weight in distance calculations.

Elbow Method (choosing K):

For each K in [2, 3, ..., 10]:
Fit KMeans(n_clusters=K, init='k-means++', n_init=10, random_state=42) on the scaled data.
Record the inertia (WCSS, the sum of squared distances from each point to its nearest centroid).
Plot inertia vs. K and look for the "elbow" where the curve stops dropping sharply.
Set the number of clusters K = 5.

Partitional Clustering (K-Means):

Train KMeans(n_clusters=5, init='k-means++', n_init=10, random_state=42) on the scaled data.
Assign each customer a cluster label and record the final inertia.
Compute the silhouette score of the K-Means labels.
Convert the centroids back to the original units using scaler.inverse_transform.

Hierarchical Clustering (Agglomerative):

Train AgglomerativeClustering(n_clusters=5, metric='euclidean', linkage='ward') on the scaled data.
Assign each customer a cluster label and compute the silhouette score.
Build the Ward linkage matrix with scipy and draw the dendrogram to show the full merge history.

Compare & Evaluate:

Print the K-Means inertia (WCSS), the K-Means silhouette score and the Agglomerative silhouette score.
The method with the higher silhouette score produces more compact and better separated clusters.
Discuss the strengths and weaknesses of each method (see README).

Visualize:

Figure 1: Elbow curve (inertia vs. K).
Figure 2: Dendrogram with Ward linkage.
Figure 3: Two side-by-side scatter plots of Annual Income vs. Spending Score, coloured by cluster:
Left: K-Means segments with the centroids marked as black X.
Right: Hierarchical segments.