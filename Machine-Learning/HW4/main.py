#!/usr/bin/env python
# coding: utf-8

# ### Program Assignment #4 - Backpropagation
# #### 0611262 曾長彥

# In[1]:


import pandas as pd
import numpy as np


# In[2]:


np.random.seed(0x611262)


# In[3]:


# Run forward propagation and record the output of every layer
def forward(X, weights):
    out = [X]
    for w in weights:
        out.append(sigmoid(np.dot(out[-1], w)))
    return out


# In[4]:


# Sigmoid function
def sigmoid(x):
    return 1.0 / (1.0 + np.exp(-x))


# In[5]:


# Sigmoid Deriv function (assume s = sigmoid(x))
def sigmoid_deriv(s):
    return s * (1 - s)


# In[6]:


# Run back propagation and return delta
def backprop(weights, out, y):
    delta = []
    error = y - out[-1]
    delta.append(error * sigmoid_deriv(out[-1]))

    for i in range(1, len(weights)):
        error = np.dot(delta[-1], weights[-i].T)
        delta.append(error * sigmoid_deriv(out[-i - 1]))

    delta.reverse()
    return delta


# In[7]:


# Update weights according to delta
def update_weights(weights, out, delta, learning_rate = 0.0001):
    for i in range(len(weights)):
        weights[i] += np.dot(out[i].T, delta[i]) * learning_rate


# In[8]:


# Read and shape data
df = pd.read_csv("./data.txt", header=None)
X = df.iloc[:, :-1].values
y = df.iloc[:,-1].values.reshape(-1, 1)


# In[9]:


# Initialize weights
weights = []
weights.append(np.random.randn(X.shape[1], 5))
weights.append(np.random.randn(5, 5))
weights.append(np.random.randn(5, 1))
weights


# In[10]:


# Main training code, iterate 2000 times
for i in range(1, 2000 + 1):
    out = forward(X, weights)
    if i == 1 or i % 100 == 0:
        print("Iteration: ", i)
        print("Loss: ", np.mean(np.square(y - out[-1])))  # Calculate mean L2-norm
        ans = np.where(out[-1] > 0.5, 1, 0)
        print(f"Accuracy: {np.sum(ans == y) / len(y) * 100:.2f}%")
        print()
    delta = backprop(weights, out, y)
    update_weights(weights, out, delta)


# In[11]:


# Plot ground truth
label_color = df.iloc[:,-1].map({0: "tab:blue", 1: "tab:orange"})
plot = df.plot.scatter(x=0, y=1, c=label_color)
x = np.linspace(*plot.get_xlim())
plot.plot(x, x, color="tab:green")
plot.set_xlabel("x")
plot.set_ylabel("y")
plot.set_title("Ground Truth")
plot


# In[12]:


# Plot result
label_color = pd.Series(np.where(out[-1] > 0.5, 1, 0).flat).map({0: "tab:blue", 1: "tab:orange"})
plot = df.plot.scatter(x=0, y=1, c=label_color)
x = np.linspace(*plot.get_xlim())
plot.plot(x, x, color="tab:green")
plot.set_xlabel("x")
plot.set_ylabel("y")
ans = np.where(out[-1] > 0.5, 1, 0)
print(f"Accuracy: {np.sum(ans == y) / len(y) * 100:.2f}%")
plot.set_title("Predict Result")
plot

