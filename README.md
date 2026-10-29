# Machine Learning Laboratory (Semester 5)

A curated and organized repository of 15 Machine Learning laboratory experiments covering core supervised learning, regression, classification, optimization, tree-based models, Support Vector Machines (SVM), and Multi-Layer Perceptron (MLP) neural networks.

Each experiment directory contains:
- **Python Source Code (`expX.py`)**: End-to-end executable program including data loading, preprocessing, model implementation/training, evaluation metrics, and matplotlib visualizations.
- **Algorithm Write-Up (`algorithm.md`)**: A simplified, short, step-by-step academic algorithm formatted for lab manuals, study notes, and viva preparation.

---

## Repository Structure

```text
ML/
├── Exp01_Linear_Regression/
│   ├── exp1.py                      # Batch Gradient Descent vs Normal Equation
│   └── algorithm.md                # Simplified algorithm write-up
├── Exp02_Polynomial_Regression/
│   ├── exp2.py                      # Polynomial regression & Bias-Variance tradeoff
│   └── algorithm.md
├── Exp03_Ridge_and_Lasso_Regression/
│   ├── exp3.py                      # OLS vs Ridge (L2) vs Lasso (L1) with 5-fold CV
│   └── algorithm.md
├── Exp04_Logistic_Regression_Scratch/
│   ├── exp4.py                      # Scratch Logistic Regression vs Scikit-Learn
│   └── algorithm.md
├── Exp05_MLE_vs_MAP/
│   ├── exp5.py                      # MLE vs MAP with Gaussian (L2) & Laplace (L1) priors
│   └── algorithm.md
├── Exp06_Feature_Scaling_Logistic/
│   ├── exp6.py                      # Impact of StandardScaler on logistic regression
│   └── algorithm.md
├── Exp07_Naive_Bayes_Text_Classification/
│   ├── exp7.py                      # Multinomial NB (Frequency) vs Bernoulli NB (Binary)
│   └── algorithm.md
├── Exp08_KNN_Fashion_MNIST/
│   ├── exp8.py                      # K-Nearest Neighbors hyperparameter tuning & latency
│   └── algorithm.md
├── Exp09_Decision_Tree_Segmentation/
│   ├── exp9.py                      # Customer segmentation using Shannon Entropy / ID3
│   └── algorithm.md
├── Exp10_Logistic_Regression_vs_Decision_Tree/
│   ├── exp10.py                     # Linear vs Tree-based model comparison on Census data
│   └── algorithm.md
├── Exp11_Linear_SVM_Decision_Boundary/
│   ├── exp11.py                     # Linear SVM hyperplane, margin, and support vectors
│   └── algorithm.md
├── Exp12_SVM_Kernels_with_PCA/
│   ├── exp12.py                     # Linear, Polynomial, and RBF kernels with PCA on Fashion-MNIST
│   └── algorithm.md
├── Exp13_MLP_Neural_Network_Architectures/
│   ├── exp13.py                     # MLP depth and hidden layer size exploration on Wine Quality
│   └── algorithm.md
├── Exp14_MLP_Activation_Functions/
│   ├── exp14.py                     # Sigmoid vs Tanh vs ReLU convergence on MNIST digits
│   └── algorithm.md
├── Exp15_MLP_Hyperparameter_Tuning/
│   ├── exp15.py                     # Grid search on Learning Rate, Batch Size, and Epochs
│   └── algorithm.md
└── requirements.txt
```

---

## Installation & Setup

1. **Clone or navigate to the repository:**
   ```bash
   git clone https://github.com/prawmathean/S5 && cd S5
   ```

    To open :

    ```
    xdg-open . 
    ```
2. **Create and activate a virtual environment (recommended):**
   ```bash
   python3 -m venv venv
   source venv/bin/activate
   ```

3. **Install dependencies:**
   ```bash
   pip install -r requirements.txt
   ```

---

## Experiments Overview

| Exp # | Experiment Title | Model / Algorithm | Dataset | Primary Metrics |
|:-----:|:-----------------|:------------------|:--------|:----------------|
| **01** | Linear Regression | Gradient Descent & Normal Equation | California Housing | MSE, RMSE, $R^2$ |
| **02** | Polynomial Regression | Polynomial Feature Mapping ($d=1\dots8$) | Auto MPG | MSE, RMSE, $R^2$ |
| **03** | Regularized Regression | OLS, Ridge (L2), Lasso (L1) with CV | Diabetes | MSE, RMSE, $R^2$, Sparsity |
| **04** | Logistic Regression | NumPy Scratch vs Scikit-Learn | Breast Cancer | Accuracy, Precision, Recall, F1 |
| **05** | MLE vs MAP Estimation | Unregularized (MLE) vs L2 & L1 MAP | Breast Cancer | Weight Norms, Zero Coefficients |
| **06** | Feature Scaling Impact | Raw Features vs StandardScaler | Pima Indians Diabetes | Accuracy, F1, Weight Analysis |
| **07** | Naïve Bayes Classification | Multinomial NB vs Bernoulli NB | 20 Newsgroups (Text) | Accuracy, Macro F1-Score |
| **08** | K-Nearest Neighbors | KNN ($K \in \{1, 3, 5, 11\}$) | Fashion-MNIST | Accuracy, Macro F1, Latency |
| **09** | Decision Tree Classifier | Shannon Entropy / ID3 Tree ($d=3$) | Synthetic RFM Segments | Accuracy, Feature Importance |
| **10** | Model Comparison | Logistic Regression vs Decision Tree | Adult Census Income | Accuracy, Precision, Recall, F1 |
| **11** | Linear SVM & Margins | Linear SVC with Support Vector plotting | Fisher's Iris | Accuracy, F1, Margin Plots |
| **12** | SVM Kernels & PCA | Linear, Polynomial, and RBF Kernels + PCA | Fashion-MNIST | Accuracy, Macro F1, Train Time |
| **13** | MLP Architectures | Neural Network Layer Topologies | Wine Quality (Red) | Accuracy, F1, Loss Curve |
| **14** | MLP Activation Functions | Sigmoid / Logistic vs Tanh vs ReLU | MNIST Hand-written Digits | Convergence Speed, Loss Trajectory |
| **15** | MLP Hyperparameter Tuning | Grid Search (Learning Rate, Batch, Epochs) | Fashion-MNIST | Accuracy %, Execution Time |

---

## Detailed Experiment Summaries & Execution

### Experiment 01: Simple Linear Regression
- **Description**: Implements simple linear regression using both iterative Batch Gradient Descent and the closed-form analytical Normal Equation ($\theta = (X^T X)^{-1} X^T y$).
- **Run Command**:
  ```bash
  python ML/Exp01_Linear_Regression/exp1.py
  ```

### Experiment 02: Polynomial Regression & Complexity Tuning
- **Description**: Evaluates polynomial model orders from degree 1 to 8 to illustrate underfitting (high bias at degree 1) and overfitting (high variance at higher degrees).
- **Run Command**:
  ```bash
  python ML/Exp02_Polynomial_Regression/exp2.py
  ```

### Experiment 03: Regularized Linear Regression (Ridge vs Lasso vs OLS)
- **Description**: Compares Ridge ($L_2$) and Lasso ($L_1$) regularized regression with 5-fold cross-validation (`RidgeCV`, `LassoCV`) against ordinary least squares, observing coefficient shrinkage and feature selection sparsity.
- **Run Command**:
  ```bash
  python ML/Exp03_Ridge_and_Lasso_Regression/exp3.py
  ```

### Experiment 04: Logistic Regression from Scratch vs Scikit-Learn
- **Description**: Implements gradient descent optimization with the sigmoid activation function ($\sigma(z) = \frac{1}{1 + e^{-z}}$) from scratch in pure NumPy and benchmarks against Scikit-Learn's `LogisticRegression`.
- **Run Command**:
  ```bash
  python ML/Exp04_Logistic_Regression_Scratch/exp4.py
  ```

### Experiment 05: Maximum Likelihood Estimation (MLE) vs Maximum A Posteriori (MAP)
- **Description**: Illustrates the Bayesian interpretation of regularizers: MLE (uniform prior), MAP with Gaussian Prior ($L_2$ regularization), and MAP with Laplace Prior ($L_1$ regularization / sparse weights).
- **Run Command**:
  ```bash
  python ML/Exp05_MLE_vs_MAP/exp5.py
  ```

### Experiment 06: Feature Scaling Impact on Logistic Regression
- **Description**: Tests the SAGA solver on unscaled vs standardized features using the Pima Indians Diabetes dataset, showcasing gradient convergence stability and unskewed feature coefficient weighting.
- **Run Command**:
  ```bash
  python ML/Exp06_Feature_Scaling_Logistic/exp6.py
  ```

### Experiment 07: Text Classification using Naïve Bayes
- **Description**: Compares token-count frequency modeling (`MultinomialNB`) with binary word occurrence modeling (`BernoulliNB`) with Laplace smoothing ($\alpha=1.0$) across 4 newsgroups.
- **Run Command**:
  ```bash
  python ML/Exp07_Naive_Bayes_Text_Classification/exp7.py
  ```

### Experiment 08: K-Nearest Neighbors (KNN) Classifier on Fashion-MNIST
- **Description**: Analyzes the trade-off between neighborhood size $K$, accuracy, and prediction latency on 784-dimensional Fashion-MNIST images.
- **Run Command**:
  ```bash
  python ML/Exp08_KNN_Fashion_MNIST/exp8.py
  ```

### Experiment 09: Customer Segmentation using Decision Tree (Entropy / ID3)
- **Description**: Fits a pre-pruned decision tree (`max_depth=3`) using Shannon Entropy / Information Gain to segment customers based on Recency, Frequency, and Monetary (RFM) tiers.
- **Run Command**:
  ```bash
  python ML/Exp09_Decision_Tree_Segmentation/exp9.py
  ```

### Experiment 10: Comparative Analysis: Logistic Regression vs Decision Tree
- **Description**: Benchmarks linear parametric classification (Logistic Regression) against non-linear orthogonal splitting (Decision Tree) on the UCI Adult Census Income dataset after dummy encoding.
- **Run Command**:
  ```bash
  python ML/Exp10_Logistic_Regression_vs_Decision_Tree/exp10.py
  ```

### Experiment 11: Linear Support Vector Machine (SVM) Decision Boundary
- **Description**: Trains a linear Support Vector Classifier on Iris petal dimensions, extracts support vectors, and plots the maximum-margin separating hyperplane and margin gutters ($w^T x + b = \pm 1$).
- **Run Command**:
  ```bash
  python ML/Exp11_Linear_SVM_Decision_Boundary/exp11.py
  ```

### Experiment 12: SVM Kernels with PCA on Fashion-MNIST
- **Description**: Reduces dimensionality from 784 down to 50 principal components via PCA and compares Linear, Polynomial ($d=3$), and Radial Basis Function (RBF) SVM kernels.
- **Run Command**:
  ```bash
  python ML/Exp12_SVM_Kernels_with_PCA/exp12.py
  ```

### Experiment 13: Multi-Layer Perceptron (MLP) Neural Network Architectures
- **Description**: Explores single-layer and deep multi-layer neural network topologies (`(4,)`, `(16,)`, `(64,)`, `(16, 8)`, `(32, 16, 8)`) for wine quality classification and tracks loss curves.
- **Run Command**:
  ```bash
  python ML/Exp13_MLP_Neural_Network_Architectures/exp13.py
  ```

### Experiment 14: Neural Network Activation Functions on MNIST Digits
- **Description**: Compares training loss convergence trajectories and gradient propagation across Logistic (Sigmoid), Tanh, and Rectified Linear Unit (ReLU) activations on the MNIST digit dataset.
- **Run Command**:
  ```bash
  python ML/Exp14_MLP_Activation_Functions/exp14.py
  ```

### Experiment 15: Hyperparameter Grid Search for Multi-Layer Perceptron
- **Description**: Performs systematic grid search over Stochastic Gradient Descent (SGD) learning rate, mini-batch size, and epoch count on Fashion-MNIST.
- **Run Command**:
  ```bash
  python ML/Exp15_MLP_Hyperparameter_Tuning/exp15.py
  ```

---

## Requirements

- Python 3.9+
- `numpy >= 1.24.0`
- `pandas >= 2.0.0`
- `matplotlib >= 3.7.0`
- `scikit-learn >= 1.3.0`
- `scipy >= 1.10.0`
- `tensorflow >= 2.13.0`
