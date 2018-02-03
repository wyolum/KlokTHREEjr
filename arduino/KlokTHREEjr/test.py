from numpy import *

def all_true(n, mask):
  i = 0;
  
  while(i < n and mask[i]):
    i += 1
  return i == n

def any_false(n, mask):
    return not all_true(n, mask)

def all_false(n, mask):
    i = 0
    while i < N and not mask[i]:
        i += 1
    return i == n
def any_true(n, mask):
    return not all_false(n, mask)

N = 4
X = ones(N)
Y = zeros(N)
print X, all_true(N, X), any_true(N, X), all_false(N, X), any_false(N, X)
print Y, all_true(N, Y), any_true(N, Y), all_false(N, Y), any_false(N, Y)

for i in range(N):
    X = ones(N)
    X[i] = 0
    Y = zeros(N)
    Y[i] = 1
    print X, all_true(N, X), any_true(N, X), all_false(N, X), any_false(N, X)
    print Y, all_true(N, Y), any_true(N, Y), all_false(N, Y), any_false(N, Y)
