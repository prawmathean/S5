from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier, plot_tree

# Step 1: Load dataset (real CSV if available, otherwise simulated data)
csv_path = Path()  # paste your path here
required_cols = ['Recency_Tier', 'Frequency_Tier', 'Monetary_Tier', 'Segment']

if csv_path.exists():
    df = pd.read_csv(csv_path)
    missing = [c for c in required_cols if c not in df.columns]
    if missing:
        raise KeyError(f'CSV is missing columns: {missing}. Found: {list(df.columns)}')
    df = df[required_cols]
else:
    print('WARNING: CSV not found -> using SIMULATED data.')
    np.random.seed(42)
    n = 800
    df = pd.DataFrame({
        'Recency_Tier': np.random.choice(
            ['Recent', 'Dormant', 'At-Risk'], n, p=[0.5, 0.3, 0.2]
        ),
        'Frequency_Tier': np.random.choice(
            ['Low', 'Medium', 'High'], n, p=[0.4, 0.4, 0.2]
        ),
        'Monetary_Tier': np.random.choice(
            ['Low', 'Medium', 'High'], n, p=[0.3, 0.5, 0.2]
        ),
    })
    # Segment derived from RFM points (+10% noise) so the tree has a pattern to learn
    points = (
        df['Recency_Tier'].map({'Dormant': 0, 'At-Risk': 1, 'Recent': 2})
        + df['Frequency_Tier'].map({'Low': 0, 'Medium': 1, 'High': 2})
        + df['Monetary_Tier'].map({'Low': 0, 'Medium': 1, 'High': 2})
    )
    df['Segment'] = np.where(points >= 5, 'Gold', np.where(points >= 3, 'Silver', 'Bronze'))
    flip = np.random.rand(n) < 0.10
    df.loc[flip, 'Segment'] = np.random.choice(['Bronze', 'Silver', 'Gold'], flip.sum())

# Step 2: Encode categorical columns
for col in ['Recency_Tier', 'Frequency_Tier', 'Monetary_Tier']:
    df[col] = df[col].astype('category').cat.codes
if df['Segment'].dtype == object:
    df['Segment'] = df['Segment'].map({'Bronze': 0, 'Silver': 1, 'Gold': 2})

X, y = df.drop('Segment', axis=1), df['Segment']
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.25, random_state=42
)
print(df.head())

# Step 3: Train decision tree (entropy / ID3-style)
clf_tree = DecisionTreeClassifier(
    criterion='entropy', max_depth=3, random_state=42
).fit(X_train, y_train)
y_pred = clf_tree.predict(X_test)
print(f'Model Classification Accuracy: {accuracy_score(y_test, y_pred):.4f}')

# Step 4: Tree plot
plt.figure(figsize=(14, 8))
plot_tree(
    clf_tree,
    feature_names=list(X.columns),
    class_names=['Bronze', 'Silver', 'Gold'],
    filled=True,
    rounded=True,
)
plt.title('Customer Segmentation Decision Tree Structure (Entropy/ID3 Split)')
plt.tight_layout()
plt.show()

# Step 5: Feature importance
feat_imp = pd.Series(clf_tree.feature_importances_, index=X.columns).sort_values(
    ascending=False
)
plt.figure(figsize=(8, 4))
feat_imp.plot(kind='bar', color='teal')
plt.ylabel('Relative Importance Scaling')
plt.title('RFM Feature Importance Breakdown')
plt.tight_layout()
plt.show()
print('\nFeature Importance Metrics:\n', feat_imp)
