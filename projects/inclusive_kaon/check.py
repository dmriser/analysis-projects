from ROOT import TFile, TH1F, TTree

f = TFile('mc.root')
tree = f.Get('events')

hup=TH1F('hup','hup',16, -180,180)
hdown=TH1F('hdown','hdown',16, -180,180)
diff=TH1F('diff','diff',16, -180,180)
total=TH1F('total','total',16, -180,180)

tree.Draw('phi_h >> hup', 'helicity>0', '')
tree.Draw('phi_h >> diff', 'helicity>0', '')
tree.Draw('phi_h >> hdown', 'helicity<0', '')
tree.Draw('phi_h >> total', '', '')

diff.Add(hdown,-1)

diff.Draw()
