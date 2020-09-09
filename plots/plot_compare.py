#!/usr/bin/env python3

import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys

sns.set()

if __name__ == '__main__':
  df1 = pd.read_csv(sys.argv[1], sep='\t')
  df2 = pd.read_csv(sys.argv[2], sep='\t')
  df3 = pd.read_csv(sys.argv[3], sep='\t')
  df4 = pd.read_csv(sys.argv[4], sep='\t')
  fixed_n = int(sys.argv[5])
  fixed_d = int(sys.argv[6])

  means_wang = df1.drop(['k', 'rep'], axis=1).groupby(['n', 'd']).mean()
  means_1 = df2.drop(['k', 'rep'], axis=1).groupby(['n', 'd']).mean()
  means_2 = df3.drop(['k', 'rep'], axis=1).groupby(['n', 'd']).mean()
  means_3 = df4.drop(['k', 'rep'], axis=1).groupby(['n', 'd']).mean()

  # Rounds are quries in Wang et al
  means_wang['round'] = means_wang['query']

  # Queries ours
  f = plt.figure()
  plt.title('Number of queries for different values of $d$ ($n=' + str(fixed_n) + '$)')
  plt.xlabel('$d$')
  plt.ylabel('#queries')
  plt.yscale('log')
  # plt.ticklabel_format(useOffset=False, style='plain')
  lbls = ['Wang and Honorio', '$C_2=1$', '$C_2=d+2$', '$C_2=(d+2)^2$']
  for (means, lbl) in zip([means_wang, means_1, means_2, means_3], lbls):
    plt.plot(means.loc[fixed_n]['query'], marker='o', label=lbl)
  plt.legend()
  plt.xticks(means_wang.loc[fixed_n]['query'].index.values)
  plt.tight_layout()
  plt.savefig('plot_queries_d.png')

  plt.figure()
  plt.title('Number of queries for different values of $n$ ($d=' + str(fixed_d) + '$)')
  plt.xlabel('$n$')
  plt.ylabel('#queries')
  plt.ticklabel_format(useOffset=False, style='plain')
  # plt.yscale('log')
  lbls = ['Wang and Honorio', '$C_2=1$', '$C_2=d+2$', '$C_2=(d+2)^2$']
  for (means, lbl) in zip([means_wang, means_1, means_2, means_3], lbls):
    queries = means.xs(fixed_d, axis=0, level=1)['query']
    try:
      queries = queries.drop(fixed_n, axis=0)
    except:
      pass
    plt.plot(queries, marker='o', label=lbl)
    plt.xticks(queries.index.values, rotation=45)
  plt.legend()
  plt.tight_layout()
  plt.savefig('plot_queries_n.png')






  # Rounds ours
  plt.figure()
  plt.title('Number of rounds for different values of $d$ ($n=' + str(fixed_n) + '$)')
  plt.xlabel('$d$')
  plt.ylabel('#rounds')
  plt.yscale('log')
  lbls = ['Wang and Honorio', '$C_2=1$', '$C_2=d+2$', '$C_2=(d+2)^2$']
  for (means, lbl) in zip([means_wang, means_1, means_2, means_3], lbls):
    plt.plot(means.loc[fixed_n]['round'], marker='o', label=lbl)
  plt.legend()
  plt.xticks(means_wang.loc[fixed_n]['round'].index.values)
  plt.tight_layout()
  plt.savefig('plot_rounds_d.png')

  plt.figure()
  plt.title('Number of rounds for different values of $n$ ($d=' + str(fixed_d) + '$)')
  plt.xlabel('$n$')
  plt.ylabel('#rounds')
  plt.yscale('log')
  lbls = ['Wang and Honorio', '$C_2=1$', '$C_2=d+2$', '$C_2=(d+2)^2$']
  for (means, lbl) in zip([means_wang, means_1, means_2, means_3], lbls):
    rounds = means.xs(fixed_d, axis=0, level=1)['round']
    try:
      rounds = rounds.drop(fixed_n, axis=0)
    except:
      pass
    plt.plot(rounds, marker='o', label=lbl)
    plt.xticks(rounds.index.values, rotation=45)
  plt.legend()
  plt.tight_layout()
  plt.savefig('plot_rounds_n.png')
