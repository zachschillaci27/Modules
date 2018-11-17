#!/usr/bin/env /Users/zschillaci/Software/miniconda3/envs/pyenv/bin/python
from methods import *

#Error Codes
# OrderedDict([(0, 'OK'), (4, 'inefficient'), (100, 'low gain'), (500, 'low gain'), (800, 'high offset'), (1000, 'unbonded'),
# (1100, 'low gain'), (1101, 'dead'), (1104, 'low gain'), (1500, 'lowgain'), (1501, 'dead'), (1504, 'low gain'), (4000, 'high noise'), 
# (4004, 'high noise'), (4100, 'low gain'), (4104, 'low gain'), (4500, 'low gain'), (4804, 'high offset'), (8000, 'partbonded')])
theBadCodes = ['low gain', 'lowgain', 'high noise', 'high offset',
               'partbonded', 'unbonded', 'inefficient', 'dead']

setPlotStyle()

print('--- All --- ')
print('Production:', str(len(production_files)))
print('Reception:', str(len(recption_files)))
print('Stave:', str(len(stave_files)))

production_files_LBL, production_files_SCIPP = GetFilesBySite(production_files)

recption_files_LBL, recption_files_SCIPP = GetFilesBySite(recption_files)

stave_files_LBL, stave_files_SCIPP = GetFilesBySite(stave_files)

print('--- LBL --- ')
print('Production:', str(len(production_files_LBL)))
print('Reception:', str(len(recption_files_LBL)))
print('Stave:', str(len(stave_files_LBL)))

print('--- SCIPP --- ')
print('Production:', str(len(production_files_SCIPP)))
print('Reception:', str(len(recption_files_SCIPP)))
print('Stave:', str(len(stave_files_SCIPP)))

Production = ABC130_Site_Results(production_dir, production_files, "Production (All)")
Reception = ABC130_Site_Results(recption_dir, recption_files, "Reception (All)")
Stave = ABC130_Site_Results(stave_dir, stave_files, "Stave (All)")

plotMultiple([Production, Reception, Stave], extension='All')
plotMultiple([Production, Reception, Stave], extension='All-Ch1792-1920', channels=[1792, 1792 + 128])

Production_LBL = ABC130_Site_Results(production_dir, production_files_LBL, "Production (LBL)")
Reception_LBL = ABC130_Site_Results(recption_dir, recption_files_LBL, "Reception (LBL)")
Stave_LBL = ABC130_Site_Results(stave_dir, stave_files_LBL, "Stave (LBL)")

plotMultiple([Production_LBL, Reception_LBL, Stave_LBL], extension='LBL')
plotMultiple([Production_LBL, Reception_LBL, Stave_LBL], extension='LBL-Ch1792-1920', channels=[1792, 1792 + 128])

Production_SCIPP = ABC130_Site_Results(production_dir, production_files_SCIPP, "Production (SCIPP)")
Reception_SCIPP = ABC130_Site_Results(recption_dir, recption_files_SCIPP, "Reception (SCIPP)")
Stave_SCIPP = ABC130_Site_Results(stave_dir, stave_files_SCIPP, "Stave (SCIPP)")

plotMultiple([Production_SCIPP, Reception_SCIPP, Stave_SCIPP], extension='SCIPP')
plotMultiple([Production_SCIPP, Reception_SCIPP, Stave_SCIPP], extension='SCIPP-Ch1792-1920', channels=[1792, 1792 + 128])

#LBL11
LBL11 = ['ABC130_M11_H3_HCC15', 'ABC130_M11_H4_HCC14']

Production_LBL11 = ABC130_Site_Results(production_dir, production_files_LBL, "Production (LBL11)", modules=LBL11)
Reception_LBL11 = ABC130_Site_Results(recption_dir, recption_files_LBL, "Reception (LBL11)", modules=LBL11)
Stave_LBL11 = ABC130_Site_Results(stave_dir, stave_files_LBL, "Stave (LBL11)", modules=LBL11)

plotMultiple([Production_LBL11, Reception_LBL11, Stave_LBL11], extension='LBL11')
plotMultiple([Production_LBL11, Reception_LBL11, Stave_LBL11], extension='LBL11-Ch1792-1920', channels=[1792, 1792 + 128])

plotMultipleVsChannel([Production_LBL11, Reception_LBL11, Stave_LBL11], extension='LBL11', channels=[1792, 1792 + 128])
