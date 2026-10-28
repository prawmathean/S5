from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import scipy.cluster.hierarchy as sch
from sklearn.cluster import AgglomerativeClustering, KMeans
from sklearn.metrics import silhouette_score
from sklearn.preprocessing import StandardScaler

# Step 1: Load dataset (real CSV if available, otherwise simulated data)
csv_path = Path(__file__).parent / 'Mall_Customers.csv'
if csv_path.exists():
    df = pd.read_csv(csv_path).rename(columns={
        'Annual Income (k$)': 'Annual_Income_k',
        'Spending Score (1-100)': 'Spending_Score',
    })
else:
    print('WARNING: Mall_Customers.csv not found -> using SIMULATED data.')
    np.random.seed(42)
    n_samples = 200
    df = pd.DataFrame({
        'CustomerID': range(1, n_samples + 1),
        'Age': np.random.randint(18, 70, n_samples),
        'Annual_Income_k': np.random.randint(15, 135, n_samples),
        'Spending_Score': np.random.randint(1, 100, n_samples),
    })
print('Dataset Overview:\n', df.head())

# Step 2 & 3: Isolate features and scale
X = df[['Annual_Income_k', 'Spending_Score']].values
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# Step 4: K-Means and the elbow method
wcss = []
k_values = range(2, 11)
for k in k_values:
    km = KMeans(n_clusters=k, init='k-means++', random_state=42, n_init=10)
    km.fit(X_scaled)
    wcss.append(km.inertia_)

OPTIMAL_K = 5
optimal_km = KMeans(n_clusters=OPTIMAL_K, init='k-means++', random_state=42, n_init=10)
kmeans_labels = optimal_km.fit_predict(X_scaled)
kmeans_sil = silhouette_score(X_scaled, kmeans_labels)

# Step 5: Hierarchical agglomerative clustering
optimal_agg = AgglomerativeClustering(n_clusters=OPTIMAL_K, metric='euclidean', linkage='ward')
agg_labels = optimal_agg.fit_predict(X_scaled)
agg_sil = silhouette_score(X_scaled, agg_labels)

# Step 6 & 7: Metrics
print(f'\nK-Means (K={OPTIMAL_K}) WCSS (Inertia): {optimal_km.inertia_:.2f}')
print(f'K-Means (K={OPTIMAL_K}) Silhouette Score: {kmeans_sil:.4f}')
print(f'Agglomerative (K={OPTIMAL_K}) Silhouette Score: {agg_sil:.4f}')

# Step 8: Elbow curve and dendrogram
plt.figure(figsize=(7, 5))
plt.plot(k_values, wcss, 'o-')
plt.title('Elbow Method (K-Means)')
plt.xlabel('Number of clusters K')
plt.ylabel('Inertia (WCSS)')
plt.show()

plt.figure(figsize=(10, 5))
sch.dendrogram(sch.linkage(X_scaled, method='ward'), no_labels=True)
plt.title('Dendrogram (Ward linkage)')
plt.xlabel('Customers')
plt.ylabel('Distance')
plt.show()

# Step 9: Segment visualizations
centers = scaler.inverse_transform(optimal_km.cluster_centers_)

plt.figure(figsize=(15, 6))

plt.subplot(1, 2, 1)
for i in range(OPTIMAL_K):
    plt.scatter(X[kmeans_labels == i, 0], X[kmeans_labels == i, 1],
                s=50, label=f'Cluster {i+1}')
plt.scatter(centers[:, 0], centers[:, 1], s=150, c='black', marker='X', label='Centroids')
plt.title('K-Means Clustering Segments')
plt.xlabel('Annual Income (k$)')
plt.ylabel('Spending Score (1-100)')
plt.legend()

plt.subplot(1, 2, 2)
for i in range(OPTIMAL_K):
    plt.scatter(X[agg_labels == i, 0], X[agg_labels == i, 1],
                s=50, label=f'Cluster {i+1}')
plt.title('Hierarchical Clustering Segments')
plt.xlabel('Annual Income (k$)')
plt.ylabel('Spending Score (1-100)')
plt.legend()

plt.tight_layout()
plt.show()