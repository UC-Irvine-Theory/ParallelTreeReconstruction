#!/usr/bin/env python3

import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys

# Make it pretty
sns.set()

if __name__ == '__main__':
  if len(sys.argv) != 3:
    print('Usage: %s <results_ours.tsv> <results_wang.tsv>' % sys.argv[0])
    sys.exit(1)

  df_ours = pd.read_csv(sys.argv[1], sep='\t')
  df_wang = pd.read_csv(sys.argv[2], sep='\t')

  # Get number of reps
  reps = max(df_ours['rep']) + 1

  # Get constant C_2
  C2 = df_ours['C2'].loc[0]
  assert(all(df_ours['C2'] == C2))

  # Drop unnecessary fields
  df_ours = df_ours.drop(['k', 'd', 'C2', 'rep'], axis=1)
  df_wang = df_wang.drop(['k', 'd', 'rep'], axis=1)

  # Average out query/rounds over reps
  # The following assumes that entries corresponding to multiple repetitions on
  # the same tree appear consecutively in the data frame. It groups by indices,
  # because there exist trees with same pair (n, d).
  df_ours['group'] = df_ours.index // reps  # merge consecutive `reps` entries
  df_wang['group'] = df_wang.index // reps  # merge consecutive `reps` entries
  df_ours = df_ours.groupby('group').mean()
  df_wang = df_wang.groupby('group').mean()
  
  # Plot
  plt.title('Reconstructions of Biological Phylogenetic Trees Using Path Queries')
  plt.scatter(df_ours['n'], df_ours['query'], label='Our Algorithm-Queries ($C_2=%d$)' % C2, marker='+')
  plt.scatter(df_ours['n'], df_ours['round'], label='Our Algorithm-Rounds ($C_2=%d$)' % C2, marker='*')
  plt.scatter(df_wang['n'], df_wang['query'], label='Wang and Honorio', marker='^')
  plt.xlabel('Number of Nodes in Tree')
  plt.ylabel('Queries/Rounds to Reconstruct')
  plt.legend(loc='upper left')
  # plt.ylim(bottom=-20000, top=600 * 1000)  # To get rid of outliers
  plt.ticklabel_format(style='plain', axis='y')
  plt.tight_layout()

  # Save plot
  output_file = 'plot_phylo_path_queries_C_is_%d.png' % C2
  print('Saving plot into %s...' % output_file)
  plt.savefig(output_file)

