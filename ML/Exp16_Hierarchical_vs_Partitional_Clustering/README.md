# Experiment 16: Customer Segmentation using K-Means and Hierarchical Clustering

## What is this experiment all about?

Imagine you run a shopping mall and want to group your customers without anyone telling you what the groups are. That is **unsupervised learning**: there are no labels, and the algorithm has to discover the structure on its own.

We cluster customers using two features:

1. **Annual Income (k$):** How much the customer earns per year.
2. **Spending Score (1-100):** How much the customer spends at the mall.

Two clustering methods are compared:

1. **K-Means:** Picks K centre points (centroids), assigns every customer to the nearest one, and keeps moving the centroids until the groups stop changing.
   * You must choose K yourself. The **Elbow Method** helps: plot inertia (WCSS) for different K and look for the bend where adding more clusters stops helping much.
2. **Agglomerative (Hierarchical) Clustering:** Starts with every customer as their own group and repeatedly merges the two closest groups (using Ward linkage) until K groups remain.
   * The **Dendrogram** shows the whole merge history as a tree.

To compare the two we use the **Silhouette Score** (from -1 to 1). Higher means customers sit closer to their own cluster and farther from the others.

## What is the code actually doing?

1. **Loads the dataset:** Generates 200 simulated customers (fixed seed 42) with Age, Annual Income and Spending Score. If `Mall_Customers.csv` is placed next to `exp16.py`, the script uses that file instead and prints a warning only when it falls back to simulated data.
2. **Scales the features:** Applies `StandardScaler` to Annual Income and Spending Score so both count equally.
3. **Runs the elbow method:** Trains K-Means for K = 2 to 10 and records the inertia (WCSS) of each.
4. **Fits the final models with K = 5:**
   * K-Means with `k-means++` initialization
   * Agglomerative clustering with Ward linkage
5. **Evaluates both:** Prints the K-Means inertia and the silhouette score of each method.
6. **Plots three figures:**
   * Elbow curve (inertia vs K)
   * Dendrogram (Ward linkage)
   * Side-by-side scatter plots of the K-Means and hierarchical clusters, with the K-Means centroids (black X) converted back to the original units

## A note on the data

The simulated customers are random, so there are no true groups hiding in them. Expect a smooth elbow curve with no sharp bend, and silhouette scores that are only moderate. K = 5 is used as specified in the lab manual. With the real Mall Customers dataset the clusters are much clearer.
