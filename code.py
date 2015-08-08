import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from numpy import linalg
import math

image = mpimg.imread('cat.png')
M = image.shape[0] # i
N = image.shape[1] # j

newImg = np.zeros((M,N))
for i in range(M):
    for j in range(N):
        newImg[i,j] = math.log(newImg[i,j]+1)
img = image
D11 = M*N
D12 = N*sum(range(1,M+1))
D21 = D12
D13 = M*sum(range(1,N+1))
D31 = D13

tmp = np.zeros(M)
for i in range(1,M+1):
    tmp[i-1] = i*sum(range(1,N+1))
D14 = sum(tmp)
D41 = D14

tempM = [x*x for x in range(1,M+1)]
D22 = N*sum(tempM)
tempN = [x*x for x in range(1,N+1)]
D33 = M*sum(tempN)
D32 = D14
D23 = D32

tmp = np.zeros(M)
for i in range(1,M+1):
    tmp[i-1] = pow(i,2)*sum(range(1,N+1))
D24 = sum(tmp)
D42 = D24

tmp = np.zeros(M)
tempN = [x*x for x in range(1,N+1)]
for i in range(1,M+1):
    tmp[i-1] = i*sum(tempN)
D34 = sum(tmp)
D43 = D34

tmp = np.zeros(M)
tempN = [x*x for x in range(1,N+1)]
for i in range(1,M+1):
    tmp[i-1] = pow(i,2)*sum(tempN)
D44 = sum(tmp)

D = ([D11,D12,D13,D14],[D21,D22,D23,D24],[D31,D32,D33,D34],[D41,D42,D43,D44])
Dinv = linalg.inv(D)



Z1 = img.sum()

temp = np.zeros(M)
for i in range(1,M+1):
    temp[i-1] = i*sum(img[i-1,:])
Z2 = sum(temp)

temp = np.zeros(N)
for j in range(1,N+1):
    temp[j-1] = j*sum(img[:,j-1])
Z3 = sum(temp)

temp = np.zeros((M,N))
for j in range(1,N+1):
    for i in range(1,M+1):
        # print i,j
        temp[i-1,j-1] = j*i*img[i-1,j-1]
Z4 = temp.sum()

Z = ([Z1,Z2,Z3,Z4])

W = np.dot(Dinv,Z)
Func = np.zeros((M,N))
for i in range(M):
    for j in range(N):
        Func[i,j] = W[0] + W[1]*i + W[2]*j + W[3]*i*j


tmp = image - Func
r = np.zeros((M,N))
for i in range(M):
    for j in range(N):
        r[i,j] = math.exp(tmp[i,j])

plt.imshow(image,cmap=plt.get_cmap('gray'))
plt.figure()
plt.imshow(Func,cmap=plt.get_cmap('gray'))
plt.figure()
plt.imshow(r,cmap=plt.get_cmap('gray'))
plt.show()
