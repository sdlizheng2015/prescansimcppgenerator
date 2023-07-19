#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/19/2023 3:50 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

"""
//output Alms data
    ps_Audi_A3_1.step(simulation, this);
    int i = 0;
    for (const auto & line : *ps_Audi_A3_1.almsUnit_Audi_A3_1_ALMS_1_Polylines){
      std::cout << std::fixed
                << "group" +   std::to_string(i) << "=" <<  line->GroupID << ";"
                << " lable" +  std::to_string(i) << "="<< line->LineLabel << ";"
                << " length" + std::to_string(i) << "="<< std::setprecision(4) << std::setw(8) << std::setfill('0') << std::left<< line->Length << ";"
                << " X" + std::to_string(i) <<"_N0 ="  << std::setprecision(4) << std::setw(8) << std::setfill('0') << std::left<< line->X.N0 << ";"
                << " X" + std::to_string(i) <<"_N1 ="  << std::setprecision(4) << std::setw(8) << std::setfill('0') << std::left<< line->X.N1 << ";"
                << " X" + std::to_string(i) <<"_N2 ="  << std::setprecision(4) << std::setw(8) << std::setfill('0') << std::left<< line->X.N2 << ";"
                << " X" + std::to_string(i) <<"_N3 ="  << std::setprecision(4) << std::setw(8) << std::setfill('0') << std::left<< line->X.N3 << ";"
                << " Y" + std::to_string(i) <<"_N0 ="  << std::setprecision(4) << std::setw(8) << std::setfill('0') << std::left<< line->Y.N0 << ";"
                << " Y" + std::to_string(i) <<"_N1 ="  << std::setprecision(4) << std::setw(8) << std::setfill('0') << std::left<< line->Y.N1 << ";"
                << " Y" + std::to_string(i) <<"_N2 ="  << std::setprecision(4) << std::setw(8) << std::setfill('0') << std::left<< line->Y.N2 << ";"
                << " Y" + std::to_string(i) <<"_N3 ="  << std::setprecision(4) << std::setw(8) << std::setfill('0') << std::left<< line->Y.N3 << ";"
                << " Z" + std::to_string(i) <<"_N0 ="  << std::setprecision(4) << std::setw(8) << std::setfill('0') << std::left<< line->Z.N0 << std::endl;
                i ++;
    }
"""

import numpy as np
import matplotlib.pyplot as plt

group0=1; lable0=1; length0=25.11670; X0_N0 =34.02800; X0_N1 =0.352000; X0_N2 =0.000000; X0_N3 =0.000000; Y0_N0 =7.960100; Y0_N1 =-0.93600; Y0_N2 =0.000000; Y0_N3 =0.000000; Z0_N0 =-1.32000
group1=2; lable1=2; length1=33.94820; X1_N0 =35.12610; X1_N1 =0.352000; X1_N2 =0.000000; X1_N3 =0.000000; Y1_N0 =14.98260; Y1_N1 =-0.93600; Y1_N2 =0.000000; Y1_N3 =0.000000; Z1_N0 =-1.32000
group2=3; lable2=5; length2=29.43880; X2_N0 =38.34850; X2_N1 =0.352000; X2_N2 =0.000000; X2_N3 =0.000000; Y2_N0 =16.35700; Y2_N1 =-0.93600; Y2_N2 =0.000000; Y2_N3 =0.000000; Z2_N0 =-1.32000
group3=4; lable3=3; length3=6.321500; X3_N0 =29.91310; X3_N1 =0.650900; X3_N2 =0.000000; X3_N3 =0.000000; Y3_N0 =12.75900; Y3_N1 =-0.75910; Y3_N2 =0.000000; Y3_N3 =0.000000; Z3_N0 =-1.32000
group4=4; lable4=3; length4=6.034200; X4_N0 =34.02800; X4_N1 =-0.35200; X4_N2 =0.000000; X4_N3 =0.000000; Y4_N0 =7.960100; Y4_N1 =0.936000; Y4_N2 =0.000000; Y4_N3 =0.000000; Z4_N0 =-1.32000
group5=5; lable5=5; length5=5.625200; X5_N0 =26.53800; X5_N1 =0.634300; X5_N2 =0.009400; X5_N3 =-0.00130; Y5_N0 =11.31940; Y5_N1 =-0.77660; Y5_N2 =0.009700; Y5_N3 =-0.00130; Z5_N0 =-1.32000
group6=5; lable6=5; length6=4.436800; X6_N0 =30.16320; X6_N1 =0.636300; X6_N2 =-0.06740; X6_N3 =0.005400; Y6_N0 =7.020700; Y6_N1 =-0.78470; Y6_N2 =-0.03960; Y6_N3 =0.003500; Z6_N0 =-1.32000
group7=5; lable7=5; length7=11.20250; X7_N0 =32.13000; X7_N1 =0.352000; X7_N2 =-0.00000; X7_N3 =0.000000; Y7_N0 =3.064400; Y7_N1 =-0.92890; Y7_N2 =-0.00190; Y7_N3 =0.000100; Z7_N0 =-1.32000
group8=5; lable8=5; length8=4.831500; X8_N0 =36.07450; X8_N1 =0.331100; X8_N2 =0.064800; X8_N3 =-0.00160; Y8_N0 =-7.42040; Y8_N1 =-0.94820; Y8_N2 =0.025600; Y8_N3 =0.002200; Z8_N0 =-1.32000
group9=5; lable9=5; length9=4.690600; X9_N0 =38.99800; X9_N1 =0.843100; X9_N2 =-0.00600; X9_N3 =-0.00460; Y9_N0 =-11.1613; Y9_N1 =-0.54040; Y9_N2 =-0.01480; Y9_N3 =-0.00350; Z9_N0 =-1.32000
group10=5; lable10=5; length10=1.286500; X10_N0 =42.33670; X10_N1 =0.481500; X10_N2 =-0.05400; X10_N3 =0.001600; Y10_N0 =-14.3791; Y10_N1 =-0.87670; Y10_N2 =-0.02910; Y10_N3 =0.003000; Z10_N0 =-1.32000
group11=6; lable11=1; length11=17.67490; X11_N0 =3.152100; X11_N1 =1.000000; X11_N2 =0.000000; X11_N3 =0.000000; Y11_N0 =-1.34450; Y11_N1 =0.000000; Y11_N2 =0.000000; Y11_N3 =0.000000; Z11_N0 =-1.32000
group12=7; lable12=2; length12=25.65690; X12_N0 =5.053500; X12_N1 =1.000000; X12_N2 =0.000000; X12_N3 =0.000000; Y12_N0 =2.155500; Y12_N1 =0.000000; Y12_N2 =0.000000; Y12_N3 =0.000000; Z12_N0 =-1.32000
group13=8; lable13=3; length13=17.45120; X13_N0 =13.25910; X13_N1 =1.000000; X13_N2 =0.000000; X13_N3 =0.000000; Y13_N0 =5.655500; Y13_N1 =0.000000; Y13_N2 =0.000000; Y13_N3 =0.000000; Z13_N0 =-1.32000
group14=9; lable14=3; length14=9.883300; X14_N0 =30.71040; X14_N1 =-1.00000; X14_N2 =0.000000; X14_N3 =0.000000; Y14_N0 =-1.34450; Y14_N1 =-0.00000; Y14_N2 =0.000000; Y14_N3 =0.000000; Z14_N0 =-1.32000
group15=9; lable15=3; length15=11.15460; X15_N0 =20.82700; X15_N1 =0.939700; X15_N2 =0.000000; X15_N3 =0.000000; Y15_N0 =-1.34450; Y15_N1 =-0.34200; Y15_N2 =0.000000; Y15_N3 =0.000000; Z15_N0 =-1.32000
group16=10; lable16=3; length16=5.877600; X16_N0 =11.35780; X16_N1 =0.991800; X16_N2 =0.002500; X16_N3 =-0.00020; Y16_N0 =-4.84450; Y16_N1 =-0.00000; Y16_N2 =0.000000; Y16_N3 =0.000000; Z16_N0 =-1.32000
group17=10; lable17=3; length17=4.562000; X17_N0 =17.23420; X17_N1 =1.011900; X17_N2 =-0.00510; X17_N3 =-0.00010; Y17_N0 =-4.84500; Y17_N1 =0.004000; Y17_N2 =-0.00730; Y17_N3 =-0.00470; Z17_N0 =-1.32000
group18=10; lable18=3; length18=2.370600; X18_N0 =21.73550; X18_N1 =0.947600; X18_N2 =-0.00250; X18_N3 =0.000200; Y18_N0 =-5.42210; Y18_N1 =-0.31380; Y18_N2 =-0.01390; Y18_N3 =0.002400; Z18_N0 =-1.32000
group19=10; lable19=3; length19=6.536500; X19_N0 =23.97040; X19_N1 =0.939700; X19_N2 =0.000000; X19_N3 =0.000000; Y19_N0 =-6.21200; Y19_N1 =-0.34190; Y19_N2 =0.000000; Y19_N3 =0.000000; Z19_N0 =-1.32000

for i in range(20):
    exec(f"distance{i} = np.linspace(0, length{i}, 1000)")
    exec(f"X{i} = X{i}_N0 + X{i}_N1 * distance{i} + X{i}_N2 * distance{i} ** 2 + X{i}_N3 * distance{i} ** 3")
    exec(f"Y{i} = Y{i}_N0 + Y{i}_N1 * distance{i} + Y{i}_N2 * distance{i} ** 2 + Y{i}_N3 * distance{i} ** 3")
    exec(f"plt.plot(X{i}, Y{i})")
plt.axis('equal')
plt.show()

