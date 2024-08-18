Running delta_bench...
-----------------------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations UserCounters...
-----------------------------------------------------------------------------------
BM_DeltaEncode/8               2.71 ns         2.71 ns    246602869 bytes_per_second=11.0047Gi/s
BM_DeltaEncode/64              20.7 ns         20.7 ns     35865709 bytes_per_second=11.5127Gi/s
BM_DeltaEncode/512              160 ns          160 ns      4381542 bytes_per_second=11.9203Gi/s
BM_DeltaEncode/4096            1288 ns         1288 ns       544997 bytes_per_second=11.8505Gi/s
BM_DeltaEncode/32768          10215 ns        10215 ns        69605 bytes_per_second=11.9499Gi/s
BM_DeltaEncode/65536          20404 ns        20404 ns        34049 bytes_per_second=11.9654Gi/s
BM_DeltaEncodeSIMD/8           4.03 ns         4.03 ns    174159463 bytes_per_second=7.39589Gi/s
BM_DeltaEncodeSIMD/64          13.0 ns         13.0 ns     52492690 bytes_per_second=18.2953Gi/s
BM_DeltaEncodeSIMD/512         96.4 ns         96.4 ns      7181749 bytes_per_second=19.7822Gi/s
BM_DeltaEncodeSIMD/4096         815 ns          815 ns       852443 bytes_per_second=18.7283Gi/s
BM_DeltaEncodeSIMD/32768       7052 ns         7052 ns        99347 bytes_per_second=17.3101Gi/s
BM_DeltaEncodeSIMD/65536      14507 ns        14507 ns        44915 bytes_per_second=16.8294Gi/s
BM_DeltaDecode/8               2.23 ns         2.23 ns    312345082 bytes_per_second=13.338Gi/s
BM_DeltaDecode/64              15.6 ns         15.6 ns     46025410 bytes_per_second=15.2422Gi/s
BM_DeltaDecode/512              120 ns          120 ns      5870356 bytes_per_second=15.9399Gi/s
BM_DeltaDecode/4096            1021 ns         1021 ns       689992 bytes_per_second=14.9517Gi/s
BM_DeltaDecode/32768           9119 ns         9119 ns        76834 bytes_per_second=13.3865Gi/s
BM_DeltaDecode/65536          18649 ns        18649 ns        37985 bytes_per_second=13.0916Gi/s
BM_DeltaDecodeSIMD/8           1.83 ns         1.83 ns    383774034 bytes_per_second=16.3039Gi/s
BM_DeltaDecodeSIMD/64          13.0 ns         13.0 ns     53501808 bytes_per_second=18.3362Gi/s
BM_DeltaDecodeSIMD/512          109 ns          109 ns      6528255 bytes_per_second=17.5731Gi/s
BM_DeltaDecodeSIMD/4096         984 ns          984 ns       718506 bytes_per_second=15.5017Gi/s
BM_DeltaDecodeSIMD/32768       7582 ns         7582 ns        93451 bytes_per_second=16.1008Gi/s
BM_DeltaDecodeSIMD/65536      15134 ns        15134 ns        46168 bytes_per_second=16.1317Gi/s
delta_bench completed. 
===========================================================================================================================
Running chunks_bench...
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
BM_ConstructChunks/8/2          0.495 ns        0.495 ns   1388966054
BM_ConstructChunks/64/2         0.506 ns        0.506 ns   1000000000
BM_ConstructChunks/512/2        0.506 ns        0.506 ns   1320498092
BM_ConstructChunks/4096/2       0.476 ns        0.476 ns   1407455574
BM_ConstructChunks/8192/2       0.511 ns        0.511 ns   1356662765
BM_ConstructChunks/8/8          0.507 ns        0.507 ns   1323211396
BM_ConstructChunks/64/8         0.504 ns        0.504 ns   1000000000
BM_ConstructChunks/512/8        0.502 ns        0.502 ns   1000000000
BM_ConstructChunks/4096/8       0.490 ns        0.490 ns   1320737273
BM_ConstructChunks/8192/8       0.502 ns        0.502 ns   1384419740
BM_ConstructChunks/8/64         0.524 ns        0.524 ns   1322054321
BM_ConstructChunks/64/64        0.506 ns        0.506 ns   1000000000
BM_ConstructChunks/512/64       0.507 ns        0.507 ns   1357002034
BM_ConstructChunks/4096/64      0.505 ns        0.505 ns   1000000000
BM_ConstructChunks/8192/64      0.506 ns        0.506 ns   1376216231
BM_ConstructChunks_BigO          0.50 (1)        0.50 (1)  
BM_ConstructChunks_RMS              2 %             2 %    
BM_IterateChunks/8/2             2.47 ns         2.47 ns    286941105
BM_IterateChunks/64/2            15.8 ns         15.8 ns     42848862
BM_IterateChunks/512/2            124 ns          124 ns      5754473
BM_IterateChunks/4096/2           945 ns          945 ns       742688
BM_IterateChunks/8192/2          1878 ns         1878 ns       346633
BM_IterateChunks/8/8             1.57 ns         1.57 ns    440488716
BM_IterateChunks/64/8            4.37 ns         4.37 ns    164325151
BM_IterateChunks/512/8           34.2 ns         34.2 ns     20495379
BM_IterateChunks/4096/8           237 ns          237 ns      2962514
BM_IterateChunks/8192/8           477 ns          477 ns      1253656
BM_IterateChunks/8/64            1.57 ns         1.57 ns    440450745
BM_IterateChunks/64/64           1.58 ns         1.58 ns    441330473
BM_IterateChunks/512/64          4.33 ns         4.33 ns    162429575
BM_IterateChunks/4096/64         35.8 ns         35.8 ns     17292268
BM_IterateChunks/8192/64         63.6 ns         63.6 ns     10358646
BM_IterateChunks_BigO            0.10 N          0.10 N    
BM_IterateChunks_RMS              153 %           153 %    
chunks_bench completed. 
===========================================================================================================================
Running data_container_bench...
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
BM_AddChannel/1           104 ns          104 ns      6704570
BM_AddChannel/2           170 ns          170 ns      4110599
BM_AddChannel/4           302 ns          302 ns      2392522
BM_AddChannel/8           584 ns          584 ns      1214158
BM_AddChannel/16         1380 ns         1380 ns       519912
BM_AddChannel/32         2886 ns         2886 ns       242728
BM_AddChannel/64         6059 ns         6059 ns       117515
BM_AddChannel/128       12430 ns        12430 ns        55494
BM_AddChannel/256       24478 ns        24477 ns        29302
BM_AddChannel/512       49195 ns        49194 ns        14514
BM_AddChannel/1024     106541 ns       106540 ns         6834
BM_AddChannel_BigO     102.03 N        102.03 N    
BM_AddChannel_RMS           7 %             7 %    
BM_AddChunk/1            16.4 ns         16.4 ns     37771809
BM_AddChunk/2            12.7 ns         12.7 ns     56309205
BM_AddChunk/4            13.3 ns         13.3 ns     52147004
BM_AddChunk/8            9.74 ns         9.74 ns     61651792
BM_AddChunk/16           10.4 ns         10.4 ns     58609187
BM_AddChunk/32           12.1 ns         12.1 ns     57769598
BM_AddChunk/64           9.44 ns         9.44 ns     60707818
BM_AddChunk/128          9.36 ns         9.36 ns     63117196
BM_AddChunk/256          13.2 ns         13.2 ns     43665262
BM_AddChunk/512          9.95 ns         9.95 ns     62053657
BM_AddChunk/1024         11.0 ns         11.0 ns     59093323
BM_AddChunk_BigO        11.59 (1)       11.59 (1)  
BM_AddChunk_RMS            18 %            18 %    
BM_AccessData/8          21.5 ns         21.5 ns     32194462
BM_AccessData/64         21.6 ns         21.6 ns     32272967
BM_AccessData/512        21.7 ns         21.7 ns     30945231
BM_AccessData/4096       21.7 ns         21.7 ns     30963929
BM_AccessData/8192       21.9 ns         21.9 ns     32093647
BM_AccessData_BigO      21.67 (1)       21.67 (1)  
BM_AccessData_RMS           1 %             1 %    
data_container_bench completed. 
===========================================================================================================================
Running runlength_bench...
--------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations
--------------------------------------------------------------------
BM_RunlengthEncode/64           60.6 ns         60.6 ns     13072555
BM_RunlengthEncode/128           111 ns          111 ns      5632719
BM_RunlengthEncode/256           192 ns          192 ns      3583993
BM_RunlengthEncode/512           444 ns          444 ns      1874753
BM_RunlengthEncode/1024          757 ns          757 ns       901166
BM_RunlengthEncode/2048         1491 ns         1491 ns       470914
BM_RunlengthEncode/4096         3015 ns         3015 ns       235028
BM_RunlengthEncode/8192         6184 ns         6184 ns       116027
BM_RunlengthEncode/16384       11911 ns        11911 ns        58569
BM_RunlengthEncode/32768       26457 ns        26457 ns        28983
BM_RunlengthEncode/65536       48189 ns        48189 ns        14771
BM_RunlengthEncode/128000      93269 ns        93268 ns         7575
BM_RunlengthEncode_BigO         0.73 N          0.73 N    
BM_RunlengthEncode_RMS             5 %             5 %    
BM_RunlengthDecode/64           77.3 ns         77.3 ns      9034415
BM_RunlengthDecode/128           151 ns          151 ns      4706885
BM_RunlengthDecode/256           295 ns          295 ns      2366275
BM_RunlengthDecode/512           589 ns          589 ns      1191917
BM_RunlengthDecode/1024         1178 ns         1178 ns       589625
BM_RunlengthDecode/2048         2356 ns         2356 ns       294686
BM_RunlengthDecode/4096         4745 ns         4745 ns       147236
BM_RunlengthDecode/8192         9433 ns         9433 ns        73778
BM_RunlengthDecode/16384       18879 ns        18879 ns        36967
BM_RunlengthDecode/32768       37529 ns        37529 ns        18465
BM_RunlengthDecode/65536       75152 ns        75153 ns         9356
BM_RunlengthDecode/128000     147275 ns       147271 ns         4760
BM_RunlengthDecode_BigO         1.15 N          1.15 N    
BM_RunlengthDecode_RMS             0 %             0 %    
runlength_bench completed. 
===========================================================================================================================
Running simple8b_bench...
----------------------------------------------------------------------------
Benchmark                                  Time             CPU   Iterations
----------------------------------------------------------------------------
BM_Simple8bEncodeUnsigned/1024         35975 ns        35975 ns        19403
BM_Simple8bEncodeUnsigned/4096        142829 ns       142829 ns         5001
BM_Simple8bEncodeUnsigned/32768      1153669 ns      1153654 ns          602
BM_Simple8bEncodeUnsigned/262144     9295702 ns      9295668 ns           75
BM_Simple8bEncodeUnsigned/1048576   36985096 ns     36985358 ns           19
BM_Simple8bEncodeUnsigned_BigO         35.28 N         35.28 N    
BM_Simple8bEncodeUnsigned_RMS              0 %             0 %    
BM_Simple8bDecodeUnsigned/1024          1345 ns         1342 ns       516491
BM_Simple8bDecodeUnsigned/4096          4551 ns         4551 ns       145607
BM_Simple8bDecodeUnsigned/32768        41323 ns        41322 ns        17035
BM_Simple8bDecodeUnsigned/262144      314106 ns       314101 ns         2230
BM_Simple8bDecodeUnsigned/1048576    1660032 ns      1660011 ns          446
BM_Simple8bDecodeUnsigned_BigO          0.08 NlgN       0.08 NlgN 
BM_Simple8bDecodeUnsigned_RMS              6 %             6 %    
BM_Simple8bEncodeSigned/1024           48409 ns        48266 ns        15305
BM_Simple8bEncodeSigned/4096          185108 ns       185107 ns         3759
BM_Simple8bEncodeSigned/32768        1471419 ns      1471424 ns          465
BM_Simple8bEncodeSigned/262144      11876038 ns     11876214 ns           59
BM_Simple8bEncodeSigned/1048576     47766479 ns     47765760 ns           15
BM_Simple8bEncodeSigned_BigO           45.54 N         45.54 N    
BM_Simple8bEncodeSigned_RMS                0 %             0 %    
BM_Simple8bDecodeSigned/1024            1713 ns         1708 ns       408001
BM_Simple8bDecodeSigned/4096            7308 ns         7308 ns       101386
BM_Simple8bDecodeSigned/32768         168183 ns       168183 ns         4192
BM_Simple8bDecodeSigned/262144       1349179 ns      1349159 ns          520
BM_Simple8bDecodeSigned/1048576      5765938 ns      5765894 ns          126
BM_Simple8bDecodeSigned_BigO            0.28 NlgN       0.28 NlgN 
BM_Simple8bDecodeSigned_RMS                2 %             2 %    
simple8b_bench completed. 
===========================================================================================================================
Running stripe_bench...
-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
BM_StripeCreation/8               0.471 ns        0.471 ns   1490991642
BM_StripeCreation/64              0.462 ns        0.462 ns   1524328833
BM_StripeCreation/512             0.463 ns        0.463 ns   1503051947
BM_StripeCreation/4096            0.473 ns        0.473 ns   1521532731
BM_StripeCreation/32768           0.465 ns        0.465 ns   1515780794
BM_StripeCreation/131072          0.470 ns        0.470 ns   1505669383
BM_StripeCreation_BigO             0.47 (1)        0.47 (1)  
BM_StripeCreation_RMS                 1 %             1 %    
BM_StripeIteration/8               3.65 ns         3.65 ns    178355764
BM_StripeIteration/64              30.7 ns         30.7 ns     23417715
BM_StripeIteration/512              241 ns          241 ns      2882568
BM_StripeIteration/4096            1870 ns         1870 ns       367631
BM_StripeIteration/32768          15169 ns        15169 ns        44603
BM_StripeIteration/131072         59913 ns        59913 ns        11430
BM_StripeIteration_BigO            0.46 N          0.46 N    
BM_StripeIteration_RMS                1 %             1 %    
BM_StripeRandomAccess/8            1.99 ns         1.99 ns    349234652
BM_StripeRandomAccess/64           15.1 ns         15.1 ns     46158645
BM_StripeRandomAccess/512           119 ns          119 ns      5836865
BM_StripeRandomAccess/4096          939 ns          939 ns       725261
BM_StripeRandomAccess/32768        7426 ns         7426 ns        93414
BM_StripeRandomAccess/131072      29440 ns        29440 ns        23750
BM_StripeRandomAccess_BigO         0.22 N          0.22 N    
BM_StripeRandomAccess_RMS             0 %             0 %    
BM_StripeReinterpret/8            0.482 ns        0.482 ns   1425044299
BM_StripeReinterpret/64           0.503 ns        0.503 ns   1466032140
BM_StripeReinterpret/512          0.484 ns        0.484 ns   1455005614
BM_StripeReinterpret/4096         0.488 ns        0.488 ns   1463613624
BM_StripeReinterpret/32768        0.483 ns        0.483 ns   1457152736
BM_StripeReinterpret/131072       0.478 ns        0.479 ns   1448538797
BM_StripeReinterpret_BigO          0.49 (1)        0.49 (1)  
BM_StripeReinterpret_RMS              2 %             2 %    
stripe_bench completed. 
===========================================================================================================================
Running xor_bench...
-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
BM_XorEncodeFlatArray/8            2.66 ns         2.66 ns    262247619
BM_XorEncodeFlatArray/64           19.3 ns         19.3 ns     35080176
BM_XorEncodeFlatArray/512           145 ns          145 ns      4767671
BM_XorEncodeFlatArray/4096         1223 ns         1223 ns       578442
BM_XorEncodeFlatArray/32768       10919 ns        10919 ns        67863
BM_XorEncodeFlatArray/131072      44486 ns        44486 ns        15783
BM_XorEncodeFlatArray_BigO         0.34 N          0.34 N    
BM_XorEncodeFlatArray_RMS             1 %             1 %    
BM_XorDecodeFlatArray/8            2.92 ns         2.92 ns    247052313
BM_XorDecodeFlatArray/64           21.1 ns         21.1 ns     33123567
BM_XorDecodeFlatArray/512           169 ns          169 ns      4114283
BM_XorDecodeFlatArray/4096         1334 ns         1334 ns       516769
BM_XorDecodeFlatArray/32768       11393 ns        11393 ns        62126
BM_XorDecodeFlatArray/131072      44868 ns        44869 ns        15943
BM_XorDecodeFlatArray_BigO         0.34 N          0.34 N    
BM_XorDecodeFlatArray_RMS             1 %             1 %    
BM_XorEncodeStripe/8               3.25 ns         3.25 ns    217569752
BM_XorEncodeStripe/64              29.8 ns         29.8 ns     23286551
BM_XorEncodeStripe/512              242 ns          242 ns      2906202
BM_XorEncodeStripe/4096            1888 ns         1888 ns       364069
BM_XorEncodeStripe/32768          15097 ns        15097 ns        45863
BM_XorEncodeStripe/131072         60876 ns        60876 ns        11513
BM_XorEncodeStripe_BigO            0.46 N          0.46 N    
BM_XorEncodeStripe_RMS                0 %             0 %    
BM_XorDecodeStripe/8               2.34 ns         2.34 ns    304622384
BM_XorDecodeStripe/64              20.9 ns         20.9 ns     34020532
BM_XorDecodeStripe/512              169 ns          169 ns      4075990
BM_XorDecodeStripe/4096            1335 ns         1335 ns       517012
BM_XorDecodeStripe/32768          11263 ns        11263 ns        61466
BM_XorDecodeStripe/131072         44768 ns        44767 ns        15629
BM_XorDecodeStripe_BigO            0.34 N          0.34 N    
BM_XorDecodeStripe_RMS                0 %             0 %    
xor_bench completed. 
===========================================================================================================================
