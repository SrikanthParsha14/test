"""
Created on 2013-1-19

@author: Administrator
"""
import doctest


def average(values):
    """Computes the arithmetic mean of a list of numbers.

    >>> print(average([20, 30, 70]))
    40.0
    """
    return sum(values) / len(values)

doctest.testmod()  # automatically validate the embedded tests
