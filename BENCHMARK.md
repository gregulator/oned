Running delta_bench...
------------------------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------
BM_DeltaEncode/8                9.31 ns         9.28 ns     68485980 bytes_per_second=3.21159Gi/s
BM_DeltaEncode/64               73.6 ns         73.6 ns      8002671 bytes_per_second=3.24105Gi/s
BM_DeltaEncode/512               586 ns          586 ns      1202101 bytes_per_second=3.25452Gi/s
BM_DeltaEncode/4096             4885 ns         4877 ns       154167 bytes_per_second=3.12902Gi/s
BM_DeltaEncode/32768           35922 ns        35922 ns        18826 bytes_per_second=3.39817Gi/s
BM_DeltaEncode/131072         151506 ns       151509 ns         4834 bytes_per_second=3.22278Gi/s
BM_DeltaEncode_BigO         32163.63 (1)    32162.87 (1)  
BM_DeltaEncode_RMS               171 %           171 %    
BM_DeltaEncodeSIMD/8            6.40 ns         6.40 ns    108027359 bytes_per_second=4.65992Gi/s
BM_DeltaEncodeSIMD/64           27.0 ns         27.0 ns     26191116 bytes_per_second=8.83114Gi/s
BM_DeltaEncodeSIMD/512           166 ns          166 ns      4185194 bytes_per_second=11.4611Gi/s
BM_DeltaEncodeSIMD/4096         1414 ns         1414 ns       521693 bytes_per_second=10.7918Gi/s
BM_DeltaEncodeSIMD/32768       11282 ns        11282 ns        61251 bytes_per_second=10.82Gi/s
BM_DeltaEncodeSIMD/131072      46444 ns        46445 ns        14857 bytes_per_second=10.5131Gi/s
BM_DeltaEncodeSIMD_BigO      9889.89 (1)     9890.14 (1)  
BM_DeltaEncodeSIMD_RMS           170 %           170 %    
BM_DeltaDecode/8                4.71 ns         4.71 ns    153698637 bytes_per_second=6.3231Gi/s
BM_DeltaDecode/64               36.4 ns         36.4 ns     19470362 bytes_per_second=6.54345Gi/s
BM_DeltaDecode/512               319 ns          319 ns      2205166 bytes_per_second=5.97756Gi/s
BM_DeltaDecode/4096             2295 ns         2295 ns       296854 bytes_per_second=6.6477Gi/s
BM_DeltaDecode/32768           18929 ns        18929 ns        38969 bytes_per_second=6.44895Gi/s
BM_DeltaDecode/131072          74498 ns        74500 ns         9829 bytes_per_second=6.55414Gi/s
BM_DeltaDecode_BigO         16013.67 (1)    16014.00 (1)  
BM_DeltaDecode_RMS               169 %           169 %    
BM_DeltaDecodeSIMD/8            4.27 ns         4.27 ns    164957588 bytes_per_second=6.97418Gi/s
BM_DeltaDecodeSIMD/64           26.3 ns         26.3 ns     27185089 bytes_per_second=9.06854Gi/s
BM_DeltaDecodeSIMD/512           201 ns          201 ns      3477273 bytes_per_second=9.48983Gi/s
BM_DeltaDecodeSIMD/4096         1553 ns         1553 ns       440292 bytes_per_second=9.82779Gi/s
BM_DeltaDecodeSIMD/32768       12931 ns        12931 ns        51266 bytes_per_second=9.44008Gi/s
BM_DeltaDecodeSIMD/131072      51357 ns        51357 ns        13297 bytes_per_second=9.50753Gi/s
BM_DeltaDecodeSIMD_BigO     11011.96 (1)    11012.09 (1)  
BM_DeltaDecodeSIMD_RMS           169 %           169 %    
delta_bench completed. 
===========================================================================================================================
Running chunks_bench...
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
BM_ConstructChunks/8/2           1.20 ns         1.20 ns    596548371
BM_ConstructChunks/64/2          1.19 ns         1.19 ns    581587601
BM_ConstructChunks/512/2         1.18 ns         1.18 ns    574475401
BM_ConstructChunks/4096/2        1.20 ns         1.20 ns    581121199
BM_ConstructChunks/8192/2        1.17 ns         1.17 ns    586798870
BM_ConstructChunks/8/8           1.15 ns         1.15 ns    592350888
BM_ConstructChunks/64/8          1.15 ns         1.15 ns    600805251
BM_ConstructChunks/512/8         1.20 ns         1.20 ns    587721655
BM_ConstructChunks/4096/8        1.19 ns         1.19 ns    593747164
BM_ConstructChunks/8192/8        1.22 ns         1.22 ns    583286184
BM_ConstructChunks/8/64          1.19 ns         1.19 ns    571954770
BM_ConstructChunks/64/64         1.16 ns         1.16 ns    611782227
BM_ConstructChunks/512/64        1.17 ns         1.17 ns    603496660
BM_ConstructChunks/4096/64       1.15 ns         1.15 ns    616570599
BM_ConstructChunks/8192/64       1.19 ns         1.19 ns    615839389
BM_ConstructChunks_BigO          1.18 (1)        1.18 (1)  
BM_ConstructChunks_RMS              2 %             2 %    
BM_IterateChunks/8/2             6.25 ns         6.25 ns    110601467
BM_IterateChunks/64/2            39.6 ns         39.6 ns     17729006
BM_IterateChunks/512/2            309 ns          309 ns      2121717
BM_IterateChunks/4096/2          2356 ns         2356 ns       298671
BM_IterateChunks/8192/2          4748 ns         4748 ns       148452
BM_IterateChunks/8/8             2.20 ns         2.20 ns    312982748
BM_IterateChunks/64/8            10.6 ns         10.6 ns     66930436
BM_IterateChunks/512/8           84.0 ns         84.0 ns      8508993
BM_IterateChunks/4096/8           611 ns          611 ns      1192360
BM_IterateChunks/8192/8          1203 ns         1203 ns       580298
BM_IterateChunks/8/64            2.29 ns         2.29 ns    306943500
BM_IterateChunks/64/64           2.23 ns         2.23 ns    314916421
BM_IterateChunks/512/64          10.7 ns         10.7 ns     64740124
BM_IterateChunks/4096/64         79.1 ns         79.1 ns      8901368
BM_IterateChunks/8192/64          152 ns          152 ns      4626940
BM_IterateChunks_BigO            0.25 N          0.25 N    
BM_IterateChunks_RMS              153 %           153 %    
chunks_bench completed. 
===========================================================================================================================
Running data_container_bench...
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
BM_AddChannel/1           311 ns          311 ns      2238471
BM_AddChannel/2           432 ns          432 ns      1617655
BM_AddChannel/4           750 ns          750 ns       972750
BM_AddChannel/8          1320 ns         1320 ns       532164
BM_AddChannel/16         2934 ns         2934 ns       234652
BM_AddChannel/32         5891 ns         5891 ns       113602
BM_AddChannel/64        12279 ns        12279 ns        60655
BM_AddChannel/128       23742 ns        23742 ns        29945
BM_AddChannel/256       46690 ns        46690 ns        14845
BM_AddChannel/512       96381 ns        96382 ns         7413
BM_AddChannel/1024     196128 ns       196130 ns         3660
BM_AddChannel_BigO     190.41 N        190.41 N    
BM_AddChannel_RMS           2 %             2 %    
BM_AddChunk/1            16.1 ns         16.1 ns     46868768
BM_AddChunk/2            12.8 ns         12.6 ns     52124133
BM_AddChunk/4            12.9 ns         12.9 ns     52824448
BM_AddChunk/8            13.3 ns         13.3 ns     50450050
BM_AddChunk/16           16.5 ns         16.5 ns     43770818
BM_AddChunk/32           13.7 ns         13.7 ns     48392806
BM_AddChunk/64           13.6 ns         13.6 ns     52697312
BM_AddChunk/128          13.7 ns         13.7 ns     52149723
BM_AddChunk/256          12.4 ns         12.4 ns     48094661
BM_AddChunk/512          15.7 ns         15.7 ns     49435377
BM_AddChunk/1024         12.4 ns         12.4 ns     51651940
BM_AddChunk_BigO        13.92 (1)       13.90 (1)  
BM_AddChunk_RMS            10 %            10 %    
BM_AccessData/8          42.1 ns         42.1 ns     16992271
BM_AccessData/64         40.8 ns         40.8 ns     16504245
BM_AccessData/512        41.9 ns         41.9 ns     17330473
BM_AccessData/4096       40.2 ns         40.2 ns     16877238
BM_AccessData/8192       40.9 ns         40.9 ns     16888790
BM_AccessData_BigO      41.17 (1)       41.17 (1)  
BM_AccessData_RMS           2 %             2 %    
data_container_bench completed. 
===========================================================================================================================
Running runlength_bench...
--------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations
--------------------------------------------------------------------
BM_RunlengthEncode/64            131 ns          131 ns      4907717
BM_RunlengthEncode/128           241 ns          241 ns      2890625
BM_RunlengthEncode/256           437 ns          437 ns      1584149
BM_RunlengthEncode/512           992 ns          992 ns       711579
BM_RunlengthEncode/1024         1811 ns         1811 ns       356557
BM_RunlengthEncode/2048         3544 ns         3544 ns       198001
BM_RunlengthEncode/4096         7343 ns         7343 ns        98128
BM_RunlengthEncode/8192        13914 ns        13914 ns        50374
BM_RunlengthEncode/16384       29404 ns        29405 ns        24495
BM_RunlengthEncode/32768       56200 ns        56152 ns        11937
BM_RunlengthEncode/65536      110984 ns       110984 ns         6290
BM_RunlengthEncode/128000     220982 ns       220988 ns         3192
BM_RunlengthEncode_BigO         1.72 N          1.72 N    
BM_RunlengthEncode_RMS             2 %             2 %    
BM_RunlengthDecode/64            170 ns          170 ns      3887176
BM_RunlengthDecode/128           341 ns          341 ns      2073053
BM_RunlengthDecode/256           665 ns          665 ns       792030
BM_RunlengthDecode/512          1341 ns         1341 ns       520232
BM_RunlengthDecode/1024         2714 ns         2714 ns       258904
BM_RunlengthDecode/2048         5355 ns         5355 ns       132086
BM_RunlengthDecode/4096        10821 ns        10821 ns        60632
BM_RunlengthDecode/8192        21418 ns        21419 ns        31318
BM_RunlengthDecode/16384       43088 ns        43089 ns        16474
BM_RunlengthDecode/32768       87249 ns        87250 ns         8076
BM_RunlengthDecode/65536      172901 ns       172881 ns         4132
BM_RunlengthDecode/128000     354272 ns       354274 ns         2099
BM_RunlengthDecode_BigO         2.74 N          2.74 N    
BM_RunlengthDecode_RMS             4 %             4 %    
runlength_bench completed. 
===========================================================================================================================
Running simple8b_bench...
----------------------------------------------------------------------------
Benchmark                                  Time             CPU   Iterations
----------------------------------------------------------------------------
BM_Simple8bEncodeUnsigned/1024         74950 ns        74938 ns         9072
BM_Simple8bEncodeUnsigned/4096        302855 ns       302779 ns         2320
BM_Simple8bEncodeUnsigned/32768      2411193 ns      2410793 ns          291
BM_Simple8bEncodeUnsigned/262144    19221293 ns     19219586 ns           37
BM_Simple8bEncodeUnsigned/1048576   79483642 ns     79129178 ns            9
BM_Simple8bEncodeUnsigned_BigO         75.65 N         75.34 N    
BM_Simple8bEncodeUnsigned_RMS              1 %             1 %    
BM_Simple8bDecodeUnsigned/1024          2266 ns         2266 ns       300801
BM_Simple8bDecodeUnsigned/4096          9286 ns         9287 ns        74465
BM_Simple8bDecodeUnsigned/32768        73055 ns        73057 ns         9425
BM_Simple8bDecodeUnsigned/262144      592288 ns       592293 ns         1153
BM_Simple8bDecodeUnsigned/1048576    2396556 ns      2396599 ns          289
BM_Simple8bDecodeUnsigned_BigO          2.28 N          2.28 N    
BM_Simple8bDecodeUnsigned_RMS              1 %             1 %    
BM_Simple8bEncodeSigned/1024           89126 ns        89127 ns         7747
BM_Simple8bEncodeSigned/4096          359262 ns       359272 ns         1930
BM_Simple8bEncodeSigned/32768        2867968 ns      2868052 ns          244
BM_Simple8bEncodeSigned/262144      23575470 ns     23576031 ns           29
BM_Simple8bEncodeSigned/1048576     91764003 ns     91765963 ns            8
BM_Simple8bEncodeSigned_BigO           87.66 N         87.66 N    
BM_Simple8bEncodeSigned_RMS                1 %             1 %    
BM_Simple8bDecodeSigned/1024            3177 ns         3177 ns       213532
BM_Simple8bDecodeSigned/4096           12961 ns        12962 ns        55243
BM_Simple8bDecodeSigned/32768         207408 ns       207414 ns         3464
BM_Simple8bDecodeSigned/262144       1722644 ns      1722677 ns          405
BM_Simple8bDecodeSigned/1048576      6884742 ns      6884896 ns          101
BM_Simple8bDecodeSigned_BigO            6.57 N          6.57 N    
BM_Simple8bDecodeSigned_RMS                0 %             0 %    
simple8b_bench completed. 
===========================================================================================================================
Running stripe_bench...
-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
BM_StripeCreation/8               0.833 ns        0.833 ns    817811466
BM_StripeCreation/64              0.826 ns        0.826 ns    837515928
BM_StripeCreation/512             0.830 ns        0.830 ns    837741449
BM_StripeCreation/4096            0.836 ns        0.836 ns    857659203
BM_StripeCreation/32768           0.849 ns        0.849 ns    847010537
BM_StripeCreation/131072          0.822 ns        0.822 ns    865250787
BM_StripeCreation_BigO             0.83 (1)        0.83 (1)  
BM_StripeCreation_RMS                 1 %             1 %    
BM_StripeIteration/8               8.22 ns         8.22 ns     84996121
BM_StripeIteration/64              69.9 ns         69.9 ns     10896570
BM_StripeIteration/512              534 ns          534 ns      1316274
BM_StripeIteration/4096            4345 ns         4345 ns       168017
BM_StripeIteration/32768          21406 ns        21406 ns        25117
BM_StripeIteration/131072         89217 ns        89218 ns         8112
BM_StripeIteration_BigO            0.68 N          0.68 N    
BM_StripeIteration_RMS                4 %             4 %    
BM_StripeRandomAccess/8            3.07 ns         3.07 ns    259433079
BM_StripeRandomAccess/64           24.9 ns         24.9 ns     30164262
BM_StripeRandomAccess/512           192 ns          192 ns      3948585
BM_StripeRandomAccess/4096         1526 ns         1526 ns       448731
BM_StripeRandomAccess/32768       10739 ns        10739 ns        61926
BM_StripeRandomAccess/131072      48585 ns        48586 ns        13264
BM_StripeRandomAccess_BigO         0.02 NlgN       0.02 NlgN 
BM_StripeRandomAccess_RMS             2 %             2 %    
BM_StripeReinterpret/8            0.658 ns        0.658 ns    809730649
BM_StripeReinterpret/64           0.737 ns        0.737 ns    946561211
BM_StripeReinterpret/512           1.05 ns         1.05 ns    652647184
BM_StripeReinterpret/4096          1.01 ns         1.01 ns    690212783
BM_StripeReinterpret/32768         1.05 ns         1.05 ns    674476462
BM_StripeReinterpret/131072        1.04 ns         1.04 ns    667261483
BM_StripeReinterpret_BigO          0.92 (1)        0.92 (1)  
BM_StripeReinterpret_RMS             18 %            18 %    
stripe_bench completed. 
===========================================================================================================================
Running xor_bench...
-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
BM_XorEncodeFlatArray/8            6.42 ns         6.42 ns    107048862
BM_XorEncodeFlatArray/64           19.5 ns         19.5 ns     35460346
BM_XorEncodeFlatArray/512           123 ns          123 ns      5724637
BM_XorEncodeFlatArray/4096         1045 ns         1045 ns       652693
BM_XorEncodeFlatArray/32768        8573 ns         8573 ns        79267
BM_XorEncodeFlatArray/131072      35436 ns        35436 ns        20294
BM_XorEncodeFlatArray_BigO         0.27 N          0.27 N    
BM_XorEncodeFlatArray_RMS             2 %             2 %    
BM_XorDecodeFlatArray/8            5.36 ns         5.36 ns    129088891
BM_XorDecodeFlatArray/64           21.8 ns         21.8 ns     30971848
BM_XorDecodeFlatArray/512           161 ns          161 ns      4291967
BM_XorDecodeFlatArray/4096         1249 ns         1249 ns       572140
BM_XorDecodeFlatArray/32768        9916 ns         9916 ns        70574
BM_XorDecodeFlatArray/131072      39861 ns        39861 ns        18226
BM_XorDecodeFlatArray_BigO         0.30 N          0.30 N    
BM_XorDecodeFlatArray_RMS             0 %             0 %    
BM_XorEncodeStripe/8               8.42 ns         8.42 ns     79008645
BM_XorEncodeStripe/64              23.6 ns         23.6 ns     29518303
BM_XorEncodeStripe/512              147 ns          147 ns      4612555
BM_XorEncodeStripe/4096            1168 ns         1168 ns       579919
BM_XorEncodeStripe/32768           9542 ns         9542 ns        74576
BM_XorEncodeStripe/131072         39358 ns        39359 ns        18299
BM_XorEncodeStripe_BigO            0.30 N          0.30 N    
BM_XorEncodeStripe_RMS                1 %             1 %    
BM_XorDecodeStripe/8               2.85 ns         2.85 ns    240046391
BM_XorDecodeStripe/64              19.9 ns         19.9 ns     34287153
BM_XorDecodeStripe/512              166 ns          166 ns      4167508
BM_XorDecodeStripe/4096            1285 ns         1285 ns       558776
BM_XorDecodeStripe/32768          10369 ns        10369 ns        65271
BM_XorDecodeStripe/131072         42649 ns        42650 ns        16627
BM_XorDecodeStripe_BigO            0.32 N          0.32 N    
BM_XorDecodeStripe_RMS                1 %             1 %    
xor_bench completed. 
===========================================================================================================================
