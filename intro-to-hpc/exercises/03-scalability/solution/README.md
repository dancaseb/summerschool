<!--
SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Discussion

The obtained data is below.

| Nodes | Runtime (s) | Resource cost (Node-s) | Speedup | Parallel efficiency | Electricity cost (Wh) | Monetary cost (€) |
| ----: | ----------: | ---------------------: | ------: | ------------------: | ---------------------: | ----------------: |
|     1 |     31.5086 |                31.5086 |  1.0000 |            100.0000 |                   5.25 |             0.053 |
|     2 |     17.0895 |                34.1789 |  1.8437 |             92.1871 |                   5.70 |             0.057 |
|     4 |      9.7534 |                39.0136 |  3.2305 |             80.7631 |                   6.50 |             0.065 |
|     8 |      5.5704 |                44.5632 |  5.6564 |             70.7054 |                   7.42 |             0.074 |
|    16 |      3.4241 |                54.7854 |  9.2020 |             57.5128 |                   9.13 |             0.091 |

The node count of 4 gives higher parallel efficiency than 75%.

Even though supercomputer resources are typically free of charge for academic users, they still consume electricity, and
addition to direct electricity costs have lots of other costs (procuring the system, cooling, adminstration, ...). Even though in these simple tests both the electricity and monetary costs are tiny, in large scale simulations these increase quite a lot. 
For example, using 100 nodes for 24 hours would mean 1440 kWh and 14400 €. As a reference, the amount electricity is roughly the same as an average one person apartment consumes in a a year in Finland.
