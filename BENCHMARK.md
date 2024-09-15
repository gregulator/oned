Running delta_bench...
------------------------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------
BM_DeltaEncode/8                3.87 ns         3.87 ns    191124923 bytes_per_second=7.69727Gi/s
BM_DeltaEncode/64               32.6 ns         32.6 ns     22757419 bytes_per_second=7.30704Gi/s
BM_DeltaEncode/512               237 ns          237 ns      2582526 bytes_per_second=8.03692Gi/s
BM_DeltaEncode/4096             1942 ns         1942 ns       379984 bytes_per_second=7.85728Gi/s
BM_DeltaEncode/32768           15007 ns        15010 ns        47157 bytes_per_second=8.13284Gi/s
BM_DeltaEncode/131072          60672 ns        60676 ns        11724 bytes_per_second=8.04738Gi/s
BM_DeltaEncode_BigO         12982.56 (1)    12983.53 (1)  
BM_DeltaEncode_RMS               169 %           169 %    
BM_DeltaEncodeSIMD/8            3.30 ns         3.30 ns    188079015 bytes_per_second=9.04294Gi/s
BM_DeltaEncodeSIMD/64           12.2 ns         12.2 ns     58509300 bytes_per_second=19.5802Gi/s
BM_DeltaEncodeSIMD/512          89.5 ns         89.5 ns      7792081 bytes_per_second=21.3163Gi/s
BM_DeltaEncodeSIMD/4096          834 ns          834 ns       824691 bytes_per_second=18.2954Gi/s
BM_DeltaEncodeSIMD/32768        7109 ns         7110 ns        97066 bytes_per_second=17.1691Gi/s
BM_DeltaEncodeSIMD/131072      29667 ns        29672 ns        23193 bytes_per_second=16.4561Gi/s
BM_DeltaEncodeSIMD_BigO      6285.80 (1)     6286.77 (1)  
BM_DeltaEncodeSIMD_RMS           171 %           171 %    
BM_DeltaDecode/8                2.28 ns         2.28 ns    301260171 bytes_per_second=13.0921Gi/s
BM_DeltaDecode/64               16.6 ns         16.6 ns     46049875 bytes_per_second=14.3488Gi/s
BM_DeltaDecode/512               120 ns          120 ns      5839177 bytes_per_second=15.9166Gi/s
BM_DeltaDecode/4096             1007 ns         1007 ns       703466 bytes_per_second=15.1518Gi/s
BM_DeltaDecode/32768            9296 ns         9298 ns        75432 bytes_per_second=13.1291Gi/s
BM_DeltaDecode/131072          37757 ns        37762 ns        17826 bytes_per_second=12.9305Gi/s
BM_DeltaDecode_BigO          8033.10 (1)     8034.24 (1)  
BM_DeltaDecode_RMS               171 %           171 %    
BM_DeltaDecodeSIMD/8            2.04 ns         2.04 ns    354248323 bytes_per_second=14.6129Gi/s
BM_DeltaDecodeSIMD/64           13.3 ns         13.3 ns     54748921 bytes_per_second=17.8637Gi/s
BM_DeltaDecodeSIMD/512           112 ns          112 ns      6623945 bytes_per_second=17.0642Gi/s
BM_DeltaDecodeSIMD/4096          929 ns          929 ns       734591 bytes_per_second=16.4272Gi/s
BM_DeltaDecodeSIMD/32768        7476 ns         7477 ns        96523 bytes_per_second=16.3252Gi/s
BM_DeltaDecodeSIMD/131072      31873 ns        31877 ns        22855 bytes_per_second=15.3176Gi/s
BM_DeltaDecodeSIMD_BigO      6734.12 (1)     6735.10 (1)  
BM_DeltaDecodeSIMD_RMS           172 %           172 %    
delta_bench completed. 
===========================================================================================================================
Running chunks_bench...
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
BM_ConstructChunks/8/2          0.485 ns        0.485 ns   1473286578
BM_ConstructChunks/64/2         0.520 ns        0.520 ns   1501127990
BM_ConstructChunks/512/2        0.472 ns        0.472 ns   1542761605
BM_ConstructChunks/4096/2       0.461 ns        0.462 ns   1462866696
BM_ConstructChunks/8192/2       0.497 ns        0.497 ns   1462219792
BM_ConstructChunks/8/8          0.464 ns        0.464 ns   1451966159
BM_ConstructChunks/64/8         0.495 ns        0.495 ns   1404252075
BM_ConstructChunks/512/8        0.518 ns        0.519 ns   1551055770
BM_ConstructChunks/4096/8       0.504 ns        0.504 ns   1514461812
BM_ConstructChunks/8192/8       0.457 ns        0.457 ns   1554494354
BM_ConstructChunks/8/64         0.457 ns        0.457 ns   1466951060
BM_ConstructChunks/64/64        0.456 ns        0.456 ns   1490325657
BM_ConstructChunks/512/64       0.487 ns        0.487 ns   1471596712
BM_ConstructChunks/4096/64      0.484 ns        0.484 ns   1419793339
BM_ConstructChunks/8192/64      0.448 ns        0.448 ns   1564524453
BM_ConstructChunks_BigO          0.48 (1)        0.48 (1)  
BM_ConstructChunks_RMS              5 %             5 %    
BM_IterateChunks/8/2             2.76 ns         2.76 ns    244987297
BM_IterateChunks/64/2            16.7 ns         16.7 ns     40006973
BM_IterateChunks/512/2            122 ns          122 ns      5701240
BM_IterateChunks/4096/2           943 ns          943 ns       759930
BM_IterateChunks/8192/2          1885 ns         1885 ns       380207
BM_IterateChunks/8/8             1.73 ns         1.73 ns    455704542
BM_IterateChunks/64/8            4.26 ns         4.26 ns    159608367
BM_IterateChunks/512/8           34.1 ns         34.1 ns     20550857
BM_IterateChunks/4096/8           239 ns          239 ns      2999617
BM_IterateChunks/8192/8           481 ns          481 ns      1465297
BM_IterateChunks/8/64            1.56 ns         1.56 ns    446372775
BM_IterateChunks/64/64           1.57 ns         1.57 ns    451338897
BM_IterateChunks/512/64          4.24 ns         4.24 ns    161545259
BM_IterateChunks/4096/64         33.6 ns         33.6 ns     20843743
BM_IterateChunks/8192/64         64.1 ns         64.1 ns     11261859
BM_IterateChunks_BigO            0.10 N          0.10 N    
BM_IterateChunks_RMS              153 %           153 %    
chunks_bench completed. 
===========================================================================================================================
Running data_container_bench...
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
BM_AddChannel/1           208 ns          208 ns      3431430
BM_AddChannel/2           265 ns          265 ns      2404471
BM_AddChannel/4           394 ns          394 ns      1768498
BM_AddChannel/8           723 ns          723 ns      1019923
BM_AddChannel/16         1555 ns         1555 ns       430646
BM_AddChannel/32         3218 ns         3219 ns       215664
BM_AddChannel/64         6327 ns         6328 ns       110826
BM_AddChannel/128       12629 ns        12631 ns        52918
BM_AddChannel/256       24637 ns        24640 ns        28953
BM_AddChannel/512       51915 ns        51923 ns        13308
BM_AddChannel/1024     100693 ns       100708 ns         6952
BM_AddChannel_BigO      98.82 N         98.83 N    
BM_AddChannel_RMS           3 %             3 %    
BM_AddChunk/1            13.3 ns         13.3 ns     53043966
BM_AddChunk/2            10.6 ns         10.6 ns     53207864
BM_AddChunk/4            10.2 ns         10.1 ns     62173202
BM_AddChunk/8            9.28 ns         9.28 ns     64891062
BM_AddChunk/16           9.42 ns         9.42 ns     58555405
BM_AddChunk/32           10.5 ns         10.3 ns     65975184
BM_AddChunk/64           9.21 ns         9.21 ns     65153310
BM_AddChunk/128          9.06 ns         9.07 ns     64242403
BM_AddChunk/256          10.3 ns         10.1 ns     64858234
BM_AddChunk/512          9.34 ns         9.34 ns     65109193
BM_AddChunk/1024         9.18 ns         9.18 ns     65430284
BM_AddChunk_BigO        10.03 (1)        9.99 (1)  
BM_AddChunk_RMS            12 %            12 %    
BM_AccessData/8          22.3 ns         22.3 ns     32468948
BM_AccessData/64         22.2 ns         22.2 ns     31266527
BM_AccessData/512        21.7 ns         21.7 ns     31753707
BM_AccessData/4096       21.7 ns         21.7 ns     31487442
BM_AccessData/8192       21.5 ns         21.5 ns     31780522
BM_AccessData_BigO      21.91 (1)       21.91 (1)  
BM_AccessData_RMS           1 %             1 %    
data_container_bench completed. 
===========================================================================================================================
Running runlength_bench...
------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations
------------------------------------------------------------------------
BM_RunlengthEncode/64               26.6 ns         26.6 ns     26879718
BM_RunlengthEncode/128              50.6 ns         50.6 ns     13906852
BM_RunlengthEncode/256               107 ns          107 ns      6784228
BM_RunlengthEncode/512               209 ns          209 ns      3413232
BM_RunlengthEncode/1024              522 ns          522 ns      1418288
BM_RunlengthEncode/2048              835 ns          835 ns       833951
BM_RunlengthEncode/4096             2438 ns         2438 ns       300521
BM_RunlengthEncode/8192             5214 ns         5215 ns       127553
BM_RunlengthEncode/16384            7501 ns         7502 ns        84660
BM_RunlengthEncode/32768           19115 ns        19118 ns        36981
BM_RunlengthEncode/65536           45552 ns        45559 ns        15476
BM_RunlengthEncode/128000          89616 ns        89629 ns         7684
BM_RunlengthEncode_BigO             0.04 NlgN       0.04 NlgN 
BM_RunlengthEncode_RMS                 7 %             7 %    
BM_RunlengthEncodeSIMD/64           28.6 ns         28.6 ns     27529975
BM_RunlengthEncodeSIMD/128          61.4 ns         61.5 ns     11360501
BM_RunlengthEncodeSIMD/256           186 ns          186 ns      3800840
BM_RunlengthEncodeSIMD/512           478 ns          478 ns      1458130
BM_RunlengthEncodeSIMD/1024          861 ns          861 ns       813980
BM_RunlengthEncodeSIMD/2048         1675 ns         1675 ns       423833
BM_RunlengthEncodeSIMD/4096         3308 ns         3309 ns       207990
BM_RunlengthEncodeSIMD/8192         7279 ns         7280 ns        97909
BM_RunlengthEncodeSIMD/16384       14573 ns        14575 ns        48937
BM_RunlengthEncodeSIMD/32768       29431 ns        29436 ns        23857
BM_RunlengthEncodeSIMD/65536       60891 ns        60900 ns        11364
BM_RunlengthEncodeSIMD/128000     118220 ns       118239 ns         5907
BM_RunlengthEncodeSIMD_BigO         0.92 N          0.92 N    
BM_RunlengthEncodeSIMD_RMS             2 %             2 %    
BM_RunlengthDecode/64               29.9 ns         29.9 ns     23785318
BM_RunlengthDecode/128              51.4 ns         51.4 ns     12854981
BM_RunlengthDecode/256               125 ns          125 ns      5424498
BM_RunlengthDecode/512               268 ns          268 ns      2772797
BM_RunlengthDecode/1024              582 ns          582 ns      1124811
BM_RunlengthDecode/2048             1248 ns         1249 ns       583666
BM_RunlengthDecode/4096             2613 ns         2614 ns       268954
BM_RunlengthDecode/8192             5411 ns         5412 ns       130218
BM_RunlengthDecode/16384           11617 ns        11619 ns        62767
BM_RunlengthDecode/32768           22166 ns        22170 ns        31238
BM_RunlengthDecode/65536           50193 ns        50201 ns        15009
BM_RunlengthDecode/128000          93822 ns        93836 ns         7417
BM_RunlengthDecode_BigO             0.74 N          0.74 N    
BM_RunlengthDecode_RMS                 5 %             5 %    
BM_RunlengthDecodeSIMD/64           16.7 ns         16.7 ns     42491043
BM_RunlengthDecodeSIMD/128          28.8 ns         28.8 ns     23791623
BM_RunlengthDecodeSIMD/256          84.2 ns         84.3 ns      8355455
BM_RunlengthDecodeSIMD/512           178 ns          178 ns      3967307
BM_RunlengthDecodeSIMD/1024          306 ns          306 ns      2284155
BM_RunlengthDecodeSIMD/2048          591 ns          591 ns      1192407
BM_RunlengthDecodeSIMD/4096         1201 ns         1201 ns       570431
BM_RunlengthDecodeSIMD/8192         2455 ns         2455 ns       283375
BM_RunlengthDecodeSIMD/16384        4808 ns         4809 ns       142980
BM_RunlengthDecodeSIMD/32768        9966 ns         9967 ns        70242
BM_RunlengthDecodeSIMD/65536       23596 ns        23599 ns        30301
BM_RunlengthDecodeSIMD/128000      52189 ns        52197 ns        13428
BM_RunlengthDecodeSIMD_BigO         0.02 NlgN       0.02 NlgN 
BM_RunlengthDecodeSIMD_RMS             8 %             8 %    
runlength_bench completed. 
===========================================================================================================================
Running simple8b_bench...
------------------------------------------------------------------------------------------
Benchmark                                                Time             CPU   Iterations
------------------------------------------------------------------------------------------
BM_Simple8bEncodeUnsigned/10/iterations:1             2623 ns         1600 ns            1
BM_Simple8bEncodeUnsigned/64/iterations:1             2861 ns         2300 ns            1
BM_Simple8bEncodeUnsigned/500/iterations:1            3099 ns         2800 ns            1
BM_Simple8bEncodeUnsigned/iterations:1_BigO         421.18 lgN      347.03 lgN  
BM_Simple8bEncodeUnsigned/iterations:1_RMS              29 %            15 %    
BM_Simple8bEncodeUnsigned_SIMD/10/iterations:1        2623 ns         2000 ns            1
BM_Simple8bEncodeUnsigned_SIMD/64/iterations:1        2146 ns         1700 ns            1
BM_Simple8bEncodeUnsigned_SIMD/500/iterations:1       2861 ns         2900 ns            1
BM_Simple8bEncodeUnsigned_SIMD/iterations:1_BigO    2543.13 (1)     2200.00 (1)  
BM_Simple8bEncodeUnsigned_SIMD/iterations:1_RMS         12 %            23 %    
ComputeSimple8bDecodeSize: 10
BM_Simple8bDecodeUnsigned/10/iterations:1             1192 ns          800 ns            1
ComputeSimple8bDecodeSize: 64
BM_Simple8bDecodeUnsigned/64/iterations:1             1907 ns         1400 ns            1
ComputeSimple8bDecodeSize: 500
BM_Simple8bDecodeUnsigned/500/iterations:1            1907 ns         1600 ns            1
BM_Simple8bDecodeUnsigned/iterations:1_BigO         255.10 lgN      199.36 lgN  
BM_Simple8bDecodeUnsigned/iterations:1_RMS              22 %            14 %    
ComputeSimple8bDecodeSize_SIMD: 10
BM_Simple8bDecodeUnsigned_SIMD/10/iterations:1         954 ns          900 ns            1
ComputeSimple8bDecodeSize_SIMD: 64
BM_Simple8bDecodeUnsigned_SIMD/64/iterations:1        1669 ns         1500 ns            1
ComputeSimple8bDecodeSize_SIMD: 500
BM_Simple8bDecodeUnsigned_SIMD/500/iterations:1       1192 ns         1100 ns            1
BM_Simple8bDecodeUnsigned_SIMD/iterations:1_BigO    1271.57 (1)     1166.67 (1)  
BM_Simple8bDecodeUnsigned_SIMD/iterations:1_RMS         23 %            21 %    
BM_Simple8bEncodeSigned/10/iterations:1               1431 ns         1000 ns            1
BM_Simple8bEncodeSigned/64/iterations:1                954 ns          900 ns            1
BM_Simple8bEncodeSigned/500/iterations:1              1907 ns         1700 ns            1
BM_Simple8bEncodeSigned/iterations:1_BigO           216.41 lgN      188.07 lgN  
BM_Simple8bEncodeSigned/iterations:1_RMS                32 %            21 %    
BM_Simple8bEncodeSigned_SIMD/10/iterations:1           954 ns          800 ns            1
BM_Simple8bEncodeSigned_SIMD/64/iterations:1          1192 ns         1000 ns            1
BM_Simple8bEncodeSigned_SIMD/500/iterations:1         1907 ns         1700 ns            1
BM_Simple8bEncodeSigned_SIMD/iterations:1_BigO      215.20 lgN      187.56 lgN  
BM_Simple8bEncodeSigned_SIMD/iterations:1_RMS           11 %            11 %    
BM_Simple8bDecodeSigned/10/iterations:1               1192 ns          900 ns            1
BM_Simple8bDecodeSigned/64/iterations:1               1669 ns         1000 ns            1
BM_Simple8bDecodeSigned/500/iterations:1              2384 ns         2300 ns            1
BM_Simple8bDecodeSigned/iterations:1_BigO           277.43 lgN      232.39 lgN  
BM_Simple8bDecodeSigned/iterations:1_RMS                10 %            19 %    
BM_Simple8bDecodeSigned_SIMD/10/iterations:1          1192 ns          900 ns            1
BM_Simple8bDecodeSigned_SIMD/64/iterations:1           954 ns         1000 ns            1
BM_Simple8bDecodeSigned_SIMD/500/iterations:1         1907 ns         1500 ns            1
BM_Simple8bDecodeSigned_SIMD/iterations:1_BigO      210.19 lgN      176.10 lgN  
BM_Simple8bDecodeSigned_SIMD/iterations:1_RMS           25 %            17 %    
simple8b_bench completed. 
===========================================================================================================================
Running stripe_bench...
-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
BM_StripeCreation/8               0.457 ns        0.457 ns   1546823565
BM_StripeCreation/64              0.450 ns        0.450 ns   1491716392
BM_StripeCreation/512             0.456 ns        0.456 ns   1525492505
BM_StripeCreation/4096            0.453 ns        0.453 ns   1547400069
BM_StripeCreation/32768           0.466 ns        0.466 ns   1371634597
BM_StripeCreation/131072          0.453 ns        0.453 ns   1532005566
BM_StripeCreation_BigO             0.46 (1)        0.46 (1)  
BM_StripeCreation_RMS                 1 %             1 %    
BM_StripeIteration/8               3.72 ns         3.72 ns    190652784
BM_StripeIteration/64              29.3 ns         29.3 ns     23925514
BM_StripeIteration/512              242 ns          242 ns      2961628
BM_StripeIteration/4096            1864 ns         1864 ns       355381
BM_StripeIteration/32768          14691 ns        14693 ns        47827
BM_StripeIteration/131072         58319 ns        58327 ns        11714
BM_StripeIteration_BigO            0.45 N          0.45 N    
BM_StripeIteration_RMS                0 %             0 %    
BM_StripeRandomAccess/8            2.00 ns         2.00 ns    358671971
BM_StripeRandomAccess/64           16.0 ns         16.0 ns     43508392
BM_StripeRandomAccess/512           124 ns          124 ns      5841838
BM_StripeRandomAccess/4096          931 ns          931 ns       737312
BM_StripeRandomAccess/32768        7627 ns         7628 ns        95088
BM_StripeRandomAccess/131072      31053 ns        31058 ns        23482
BM_StripeRandomAccess_BigO         0.24 N          0.24 N    
BM_StripeRandomAccess_RMS             1 %             1 %    
BM_StripeReinterpret/8            0.527 ns        0.527 ns   1343773816
BM_StripeReinterpret/64           0.476 ns        0.476 ns   1442697201
BM_StripeReinterpret/512          0.486 ns        0.486 ns   1436758902
BM_StripeReinterpret/4096         0.508 ns        0.508 ns   1428288103
BM_StripeReinterpret/32768        0.552 ns        0.552 ns   1000000000
BM_StripeReinterpret/131072       0.483 ns        0.483 ns   1416482187
BM_StripeReinterpret_BigO          0.51 (1)        0.51 (1)  
BM_StripeReinterpret_RMS              5 %             5 %    
stripe_bench completed. 
===========================================================================================================================
Running xor_bench...
-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
BM_XorEncodeFlatArray/8            3.37 ns         3.37 ns    230048018
BM_XorEncodeFlatArray/64           11.0 ns         11.0 ns     58839451
BM_XorEncodeFlatArray/512          74.4 ns         74.3 ns      9389999
BM_XorEncodeFlatArray/4096          664 ns          664 ns      1033868
BM_XorEncodeFlatArray/32768        5946 ns         5947 ns       121044
BM_XorEncodeFlatArray/131072      22351 ns        22354 ns        32295
BM_XorEncodeFlatArray_BigO         0.17 N          0.17 N    
BM_XorEncodeFlatArray_RMS             3 %             3 %    
BM_XorDecodeFlatArray/8            6.48 ns         6.48 ns    105018378
BM_XorDecodeFlatArray/64           10.8 ns         10.8 ns     68384819
BM_XorDecodeFlatArray/512          86.5 ns         86.6 ns      8888832
BM_XorDecodeFlatArray/4096          626 ns          627 ns      1155529
BM_XorDecodeFlatArray/32768        5413 ns         5413 ns       134019
BM_XorDecodeFlatArray/131072      21218 ns        21221 ns        31336
BM_XorDecodeFlatArray_BigO         0.16 N          0.16 N    
BM_XorDecodeFlatArray_RMS             1 %             1 %    
BM_XorEncodeStripe/8               4.24 ns         4.24 ns    169266111
BM_XorEncodeStripe/64              12.3 ns         12.3 ns     53980232
BM_XorEncodeStripe/512             79.5 ns         79.5 ns      8961239
BM_XorEncodeStripe/4096             644 ns          644 ns      1067371
BM_XorEncodeStripe/32768           5897 ns         5898 ns       125242
BM_XorEncodeStripe/131072         23781 ns        23784 ns        29876
BM_XorEncodeStripe_BigO            0.18 N          0.18 N    
BM_XorEncodeStripe_RMS                1 %             1 %    
BM_XorDecodeStripe/8               1.40 ns         1.40 ns    501426199
BM_XorDecodeStripe/64              11.3 ns         11.3 ns     62365635
BM_XorDecodeStripe/512             98.0 ns         98.0 ns      7046592
BM_XorDecodeStripe/4096             804 ns          804 ns       926934
BM_XorDecodeStripe/32768           6512 ns         6513 ns       112059
BM_XorDecodeStripe/131072         27038 ns        27043 ns        27444
BM_XorDecodeStripe_BigO            0.21 N          0.21 N    
BM_XorDecodeStripe_RMS                2 %             2 %    
xor_bench completed. 
===========================================================================================================================
