Running delta_bench...
------------------------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------
BM_DeltaEncode/8                9.02 ns         9.02 ns     78642407 bytes_per_second=3.30345Gi/s
BM_DeltaEncode/64               71.7 ns         71.7 ns      9727613 bytes_per_second=3.32407Gi/s
BM_DeltaEncode/512               582 ns          582 ns      1212863 bytes_per_second=3.27726Gi/s
BM_DeltaEncode/4096             4618 ns         4618 ns       152986 bytes_per_second=3.30386Gi/s
BM_DeltaEncode/32768           37264 ns        37265 ns        18893 bytes_per_second=3.27577Gi/s
BM_DeltaEncode/131072         150058 ns       150062 ns         4335 bytes_per_second=3.25386Gi/s
BM_DeltaEncode_BigO         32100.48 (1)    32101.31 (1)  
BM_DeltaEncode_RMS               169 %           169 %    
BM_DeltaEncodeSIMD/8            6.69 ns         6.69 ns    105366147 bytes_per_second=4.4534Gi/s
BM_DeltaEncodeSIMD/64           23.6 ns         23.6 ns     29571775 bytes_per_second=10.1226Gi/s
BM_DeltaEncodeSIMD/512           169 ns          169 ns      4197116 bytes_per_second=11.3134Gi/s
BM_DeltaEncodeSIMD/4096         1467 ns         1467 ns       495537 bytes_per_second=10.3988Gi/s
BM_DeltaEncodeSIMD/32768       11520 ns        11520 ns        60259 bytes_per_second=10.5964Gi/s
BM_DeltaEncodeSIMD/131072      48494 ns        48494 ns        14757 bytes_per_second=10.0688Gi/s
BM_DeltaEncodeSIMD_BigO     10279.96 (1)    10280.07 (1)  
BM_DeltaEncodeSIMD_RMS           171 %           171 %    
BM_DeltaDecode/8                4.70 ns         4.70 ns    149529740 bytes_per_second=6.34013Gi/s
BM_DeltaDecode/64               35.7 ns         35.7 ns     19579729 bytes_per_second=6.68466Gi/s
BM_DeltaDecode/512               286 ns          286 ns      2501618 bytes_per_second=6.65942Gi/s
BM_DeltaDecode/4096             2277 ns         2277 ns       312647 bytes_per_second=6.70027Gi/s
BM_DeltaDecode/32768           17692 ns        17693 ns        39211 bytes_per_second=6.89939Gi/s
BM_DeltaDecode/131072          70668 ns        70670 ns         9854 bytes_per_second=6.90932Gi/s
BM_DeltaDecode_BigO         15160.78 (1)    15161.17 (1)  
BM_DeltaDecode_RMS               169 %           169 %    
BM_DeltaDecodeSIMD/8            4.30 ns         4.30 ns    159567363 bytes_per_second=6.92783Gi/s
BM_DeltaDecodeSIMD/64           22.6 ns         22.6 ns     27115598 bytes_per_second=10.527Gi/s
BM_DeltaDecodeSIMD/512           165 ns          165 ns      4376316 bytes_per_second=11.5305Gi/s
BM_DeltaDecodeSIMD/4096         1211 ns         1211 ns       558340 bytes_per_second=12.5996Gi/s
BM_DeltaDecodeSIMD/32768       10212 ns        10212 ns        74827 bytes_per_second=11.9531Gi/s
BM_DeltaDecodeSIMD/131072      43955 ns        43956 ns        14976 bytes_per_second=11.1084Gi/s
BM_DeltaDecodeSIMD_BigO      9261.73 (1)     9261.99 (1)  
BM_DeltaDecodeSIMD_RMS           172 %           172 %    
delta_bench completed. 
===========================================================================================================================
Running chunks_bench...
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
BM_ConstructChunks/8/2          0.844 ns        0.844 ns    679822858
BM_ConstructChunks/64/2         0.847 ns        0.847 ns    832099053
BM_ConstructChunks/512/2        0.816 ns        0.816 ns    849523842
BM_ConstructChunks/4096/2       0.763 ns        0.763 ns    730957513
BM_ConstructChunks/8192/2       0.814 ns        0.814 ns    679653881
BM_ConstructChunks/8/8          0.738 ns        0.738 ns    691117755
BM_ConstructChunks/64/8          1.13 ns         1.13 ns    619972137
BM_ConstructChunks/512/8         1.17 ns         1.17 ns    614718288
BM_ConstructChunks/4096/8        1.15 ns         1.15 ns    609301775
BM_ConstructChunks/8192/8        1.13 ns         1.13 ns    612334517
BM_ConstructChunks/8/64          1.13 ns         1.13 ns    611591405
BM_ConstructChunks/64/64         1.13 ns         1.13 ns    610509662
BM_ConstructChunks/512/64        1.16 ns         1.16 ns    601959982
BM_ConstructChunks/4096/64       1.14 ns         1.14 ns    596187128
BM_ConstructChunks/8192/64       1.13 ns         1.13 ns    591277142
BM_ConstructChunks_BigO          1.01 (1)        1.01 (1)  
BM_ConstructChunks_RMS             17 %            17 %    
BM_IterateChunks/8/2             6.04 ns         6.04 ns    116749169
BM_IterateChunks/64/2            37.9 ns         37.9 ns     18060163
BM_IterateChunks/512/2            311 ns          311 ns      2362181
BM_IterateChunks/4096/2          2291 ns         2291 ns       304780
BM_IterateChunks/8192/2          4732 ns         4733 ns       144155
BM_IterateChunks/8/8             2.21 ns         2.21 ns    283490907
BM_IterateChunks/64/8            10.6 ns         10.6 ns     66585622
BM_IterateChunks/512/8           82.0 ns         82.0 ns      9090815
BM_IterateChunks/4096/8           596 ns          597 ns      1132431
BM_IterateChunks/8192/8          1184 ns         1184 ns       577862
BM_IterateChunks/8/64            2.22 ns         2.22 ns    318189194
BM_IterateChunks/64/64           2.21 ns         2.21 ns    307717872
BM_IterateChunks/512/64          10.7 ns         10.7 ns     66657715
BM_IterateChunks/4096/64         77.7 ns         77.7 ns      8085176
BM_IterateChunks/8192/64          156 ns          156 ns      4723197
BM_IterateChunks_BigO            0.25 N          0.25 N    
BM_IterateChunks_RMS              154 %           154 %    
chunks_bench completed. 
===========================================================================================================================
Running data_container_bench...
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
BM_AddChannel/1           315 ns          315 ns      2228340
BM_AddChannel/2           432 ns          431 ns      1596451
BM_AddChannel/4           699 ns          699 ns      1017096
BM_AddChannel/8          1260 ns         1260 ns       528764
BM_AddChannel/16         2867 ns         2867 ns       235177
BM_AddChannel/32         5861 ns         5861 ns       118737
BM_AddChannel/64        11618 ns        11618 ns        60851
BM_AddChannel/128       23534 ns        23535 ns        30228
BM_AddChannel/256       45997 ns        45998 ns        14988
BM_AddChannel/512       93261 ns        93262 ns         7533
BM_AddChannel/1024     191871 ns       191874 ns         3364
BM_AddChannel_BigO     185.97 N        185.97 N    
BM_AddChannel_RMS           3 %             3 %    
BM_AddChunk/1            14.8 ns         14.8 ns     50366433
BM_AddChunk/2            14.5 ns         14.4 ns     44523966
BM_AddChunk/4            12.3 ns         12.3 ns     51216427
BM_AddChunk/8            12.4 ns         12.4 ns     48220359
BM_AddChunk/16           15.5 ns         15.3 ns     50562727
BM_AddChunk/32           12.4 ns         12.4 ns     49112812
BM_AddChunk/64           12.4 ns         12.4 ns     49045917
BM_AddChunk/128          15.3 ns         15.3 ns     43628413
BM_AddChunk/256          12.7 ns         12.7 ns     48598658
BM_AddChunk/512          13.1 ns         13.1 ns     47949212
BM_AddChunk/1024         13.5 ns         13.5 ns     38431385
BM_AddChunk_BigO        13.53 (1)       13.52 (1)  
BM_AddChunk_RMS             9 %             9 %    
BM_AccessData/8          40.4 ns         40.4 ns     17548187
BM_AccessData/64         42.2 ns         42.2 ns     16526452
BM_AccessData/512        40.9 ns         40.9 ns     15686749
BM_AccessData/4096       44.7 ns         44.7 ns     17239728
BM_AccessData/8192       40.3 ns         40.3 ns     17366504
BM_AccessData_BigO      41.70 (1)       41.70 (1)  
BM_AccessData_RMS           4 %             4 %    
data_container_bench completed. 
===========================================================================================================================
Running runlength_bench...
------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations
------------------------------------------------------------------------
BM_RunlengthEncode/64               59.7 ns         59.7 ns     11969925
BM_RunlengthEncode/128               108 ns          108 ns      6823457
BM_RunlengthEncode/256               237 ns          237 ns      2909868
BM_RunlengthEncode/512               506 ns          506 ns      1000000
BM_RunlengthEncode/1024             1005 ns         1005 ns       684844
BM_RunlengthEncode/2048             1860 ns         1860 ns       369954
BM_RunlengthEncode/4096             4405 ns         4405 ns       159644
BM_RunlengthEncode/8192             9124 ns         9124 ns        75231
BM_RunlengthEncode/16384           16364 ns        16364 ns        42450
BM_RunlengthEncode/32768           34971 ns        34971 ns        19744
BM_RunlengthEncode/65536           80069 ns        80072 ns         8711
BM_RunlengthEncode/128000         154680 ns       154684 ns         4467
BM_RunlengthEncode_BigO             0.07 NlgN       0.07 NlgN 
BM_RunlengthEncode_RMS                 6 %             6 %    
BM_RunlengthEncodeSIMD/64           49.1 ns         49.1 ns     14003960
BM_RunlengthEncodeSIMD/128          75.1 ns         75.1 ns      9403938
BM_RunlengthEncodeSIMD/256           231 ns          231 ns      3068731
BM_RunlengthEncodeSIMD/512           541 ns          541 ns      1307881
BM_RunlengthEncodeSIMD/1024          966 ns          966 ns       716766
BM_RunlengthEncodeSIMD/2048         1900 ns         1900 ns       362955
BM_RunlengthEncodeSIMD/4096         3922 ns         3922 ns       172995
BM_RunlengthEncodeSIMD/8192         8289 ns         8289 ns        76104
BM_RunlengthEncodeSIMD/16384       16576 ns        16576 ns        41759
BM_RunlengthEncodeSIMD/32768       33077 ns        33078 ns        21136
BM_RunlengthEncodeSIMD/65536       68485 ns        68487 ns        10435
BM_RunlengthEncodeSIMD/128000     132775 ns       132779 ns         5084
BM_RunlengthEncodeSIMD_BigO         1.04 N          1.04 N    
BM_RunlengthEncodeSIMD_RMS             2 %             2 %    
BM_RunlengthDecode/64               60.6 ns         60.6 ns     12135922
BM_RunlengthDecode/128               110 ns          110 ns      6280314
BM_RunlengthDecode/256               245 ns          245 ns      2799570
BM_RunlengthDecode/512               506 ns          506 ns      1283403
BM_RunlengthDecode/1024             1085 ns         1085 ns       652282
BM_RunlengthDecode/2048             2134 ns         2134 ns       333875
BM_RunlengthDecode/4096             4483 ns         4483 ns       153867
BM_RunlengthDecode/8192             9515 ns         9515 ns        76289
BM_RunlengthDecode/16384           18930 ns        18930 ns        36585
BM_RunlengthDecode/32768           37635 ns        37636 ns        18088
BM_RunlengthDecode/65536           78145 ns        78146 ns         8899
BM_RunlengthDecode/128000         153197 ns       153198 ns         4390
BM_RunlengthDecode_BigO             1.19 N          1.19 N    
BM_RunlengthDecode_RMS                 2 %             2 %    
BM_RunlengthDecodeSIMD/64           27.5 ns         27.5 ns     25771213
BM_RunlengthDecodeSIMD/128          46.0 ns         46.0 ns     15201014
BM_RunlengthDecodeSIMD/256           132 ns          132 ns      5265517
BM_RunlengthDecodeSIMD/512           285 ns          285 ns      2439689
BM_RunlengthDecodeSIMD/1024          507 ns          507 ns      1256635
BM_RunlengthDecodeSIMD/2048         1132 ns         1132 ns       611145
BM_RunlengthDecodeSIMD/4096         2251 ns         2251 ns       307797
BM_RunlengthDecodeSIMD/8192         4707 ns         4707 ns       148405
BM_RunlengthDecodeSIMD/16384        9209 ns         9209 ns        73929
BM_RunlengthDecodeSIMD/32768       18697 ns        18698 ns        35813
BM_RunlengthDecodeSIMD/65536       40816 ns        40817 ns        17693
BM_RunlengthDecodeSIMD/128000      89378 ns        89381 ns         8207
BM_RunlengthDecodeSIMD_BigO         0.04 NlgN       0.04 NlgN 
BM_RunlengthDecodeSIMD_RMS             5 %             5 %    
runlength_bench completed. 
===========================================================================================================================
Running simple8b_bench...
----------------------------------------------------------------------------
Benchmark                                  Time             CPU   Iterations
----------------------------------------------------------------------------
BM_Simple8bEncodeUnsigned/1024         75568 ns        75569 ns         9431
BM_Simple8bEncodeUnsigned/4096        303163 ns       303099 ns         2322
BM_Simple8bEncodeUnsigned/32768      2380726 ns      2380749 ns          291
BM_Simple8bEncodeUnsigned/262144    19714057 ns     19714269 ns           36
BM_Simple8bEncodeUnsigned/1048576   76878018 ns     76878678 ns            9
BM_Simple8bEncodeUnsigned_BigO         73.43 N         73.43 N    
BM_Simple8bEncodeUnsigned_RMS              1 %             1 %    
BM_Simple8bDecodeUnsigned/1024          2374 ns         2374 ns       299413
BM_Simple8bDecodeUnsigned/4096          9318 ns         9307 ns        71312
BM_Simple8bDecodeUnsigned/32768        74377 ns        74378 ns         9339
BM_Simple8bDecodeUnsigned/262144      625064 ns       625073 ns         1172
BM_Simple8bDecodeUnsigned/1048576    2510990 ns      2510999 ns          287
BM_Simple8bDecodeUnsigned_BigO          2.39 N          2.39 N    
BM_Simple8bDecodeUnsigned_RMS              0 %             0 %    
BM_Simple8bEncodeSigned/1024           92679 ns        92438 ns         7472
BM_Simple8bEncodeSigned/4096          358164 ns       358169 ns         1976
BM_Simple8bEncodeSigned/32768        2835939 ns      2836010 ns          249
BM_Simple8bEncodeSigned/262144      23242597 ns     23243079 ns           29
BM_Simple8bEncodeSigned/1048576     93225683 ns     93227757 ns            7
BM_Simple8bEncodeSigned_BigO           88.89 N         88.89 N    
BM_Simple8bEncodeSigned_RMS                0 %             0 %    
BM_Simple8bDecodeSigned/1024            3244 ns         3244 ns       208611
BM_Simple8bDecodeSigned/4096           13434 ns        13418 ns        52888
BM_Simple8bDecodeSigned/32768         209660 ns       209663 ns         3281
BM_Simple8bDecodeSigned/262144       1752714 ns      1752710 ns          403
BM_Simple8bDecodeSigned/1048576      7161526 ns      7161676 ns           97
BM_Simple8bDecodeSigned_BigO            6.82 N          6.82 N    
BM_Simple8bDecodeSigned_RMS                1 %             1 %    
simple8b_bench completed. 
===========================================================================================================================
Running stripe_bench...
-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
BM_StripeCreation/8               0.831 ns        0.831 ns    831747073
BM_StripeCreation/64              0.833 ns        0.833 ns    826754728
BM_StripeCreation/512             0.857 ns        0.857 ns    828236720
BM_StripeCreation/4096            0.829 ns        0.829 ns    830842783
BM_StripeCreation/32768           0.837 ns        0.837 ns    829278753
BM_StripeCreation/131072          0.870 ns        0.870 ns    877531679
BM_StripeCreation_BigO             0.84 (1)        0.84 (1)  
BM_StripeCreation_RMS                 2 %             2 %    
BM_StripeIteration/8               8.91 ns         8.91 ns     56856948
BM_StripeIteration/64              68.3 ns         68.3 ns      9627326
BM_StripeIteration/512              542 ns          542 ns      1329717
BM_StripeIteration/4096            4498 ns         4498 ns       150716
BM_StripeIteration/32768          33303 ns        33304 ns        20577
BM_StripeIteration/131072        138807 ns       138813 ns         5053
BM_StripeIteration_BigO            1.06 N          1.06 N    
BM_StripeIteration_RMS                2 %             2 %    
BM_StripeRandomAccess/8            4.30 ns         4.30 ns    160967460
BM_StripeRandomAccess/64           35.4 ns         35.4 ns     20021555
BM_StripeRandomAccess/512           277 ns          277 ns      2584214
BM_StripeRandomAccess/4096         1465 ns         1465 ns       398663
BM_StripeRandomAccess/32768       12154 ns        12154 ns        47378
BM_StripeRandomAccess/131072      44059 ns        44060 ns        11464
BM_StripeRandomAccess_BigO         0.34 N          0.34 N    
BM_StripeRandomAccess_RMS             5 %             5 %    
BM_StripeReinterpret/8            0.732 ns        0.732 ns    951928948
BM_StripeReinterpret/64           0.809 ns        0.809 ns    684861814
BM_StripeReinterpret/512          0.725 ns        0.725 ns    693808453
BM_StripeReinterpret/4096         0.732 ns        0.732 ns    956122187
BM_StripeReinterpret/32768        0.736 ns        0.736 ns    941239747
BM_StripeReinterpret/131072       0.706 ns        0.706 ns    908001308
BM_StripeReinterpret_BigO          0.74 (1)        0.74 (1)  
BM_StripeReinterpret_RMS              4 %             4 %    
stripe_bench completed. 
===========================================================================================================================
Running xor_bench...
-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
BM_XorEncodeFlatArray/8            5.07 ns         5.07 ns    145792997
BM_XorEncodeFlatArray/64           17.9 ns         17.9 ns     45654834
BM_XorEncodeFlatArray/512           125 ns          125 ns      5684439
BM_XorEncodeFlatArray/4096         1048 ns         1048 ns       644412
BM_XorEncodeFlatArray/32768        8418 ns         8418 ns        82853
BM_XorEncodeFlatArray/131072      33127 ns        33128 ns        21630
BM_XorEncodeFlatArray_BigO         0.25 N          0.25 N    
BM_XorEncodeFlatArray_RMS             1 %             1 %    
BM_XorDecodeFlatArray/8            4.78 ns         4.78 ns    145287416
BM_XorDecodeFlatArray/64           21.3 ns         21.3 ns     32940727
BM_XorDecodeFlatArray/512           161 ns          161 ns      4171494
BM_XorDecodeFlatArray/4096         1212 ns         1212 ns       463539
BM_XorDecodeFlatArray/32768       10000 ns        10000 ns        71853
BM_XorDecodeFlatArray/131072      39066 ns        39066 ns        17699
BM_XorDecodeFlatArray_BigO         0.30 N          0.30 N    
BM_XorDecodeFlatArray_RMS             1 %             1 %    
BM_XorEncodeStripe/8               8.37 ns         8.37 ns     85422837
BM_XorEncodeStripe/64              23.7 ns         23.7 ns     29807516
BM_XorEncodeStripe/512              148 ns          148 ns      4719102
BM_XorEncodeStripe/4096            1168 ns         1168 ns       569355
BM_XorEncodeStripe/32768           9248 ns         9248 ns        75359
BM_XorEncodeStripe/131072         39409 ns        39409 ns        18320
BM_XorEncodeStripe_BigO            0.30 N          0.30 N    
BM_XorEncodeStripe_RMS                3 %             3 %    
BM_XorDecodeStripe/8               2.83 ns         2.83 ns    250435579
BM_XorDecodeStripe/64              19.7 ns         19.7 ns     35389783
BM_XorDecodeStripe/512              166 ns          166 ns      4205101
BM_XorDecodeStripe/4096            1289 ns         1289 ns       552673
BM_XorDecodeStripe/32768          10274 ns        10274 ns        69320
BM_XorDecodeStripe/131072         41166 ns        41167 ns        17184
BM_XorDecodeStripe_BigO            0.31 N          0.31 N    
BM_XorDecodeStripe_RMS                0 %             0 %    
xor_bench completed. 
===========================================================================================================================
