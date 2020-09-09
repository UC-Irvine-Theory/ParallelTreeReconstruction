#!/usr/bin/env python3

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
import sys

sns.set()

if __name__ == '__main__':
  df = pd.read_csv(sys.stdin, sep='\t')
  fixed_n = int(sys.argv[1])
  fixed_d = int(sys.argv[2])

  means = df.drop(['k', 'rep'], axis=1).groupby(['n', 'd', 'C2']).mean()
  print(means.head())

  queries = means.loc[fixed_n, fixed_d]['query']
  rounds = means.loc[fixed_n, fixed_d]['round']

  # Queries
  plt.figure()
  plt.title('Number of queries for different values of $C_2$')
  plt.xlabel('Constant $C_2$')
  plt.ylabel('#queries')
  plt.xscale('log')
  plt.plot(queries, marker='o')
  plt.xticks(queries.index.values)
  plt.gca().get_xaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())
  plt.tight_layout()
  plt.savefig('plot_constants_queries.png')

  # Rounds
  plt.figure()
  plt.title('Number of rounds for different values of $C_2$')
  plt.xlabel('Constant $C_2$')
  plt.ylabel('#rounds')
  plt.xscale('log')
  plt.plot(rounds, marker='o')
  plt.xticks(rounds.index.values)
  plt.gca().get_xaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())
  plt.tight_layout()
  plt.savefig('plot_constants_rounds.png')
