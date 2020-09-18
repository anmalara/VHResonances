#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/Utils.h"
#include "UHH2/common/include/JetIds.h"
#include "UHH2/VHResonances/include/HiggsToWWHists.h"

#include "TH1F.h"
#include "TH2F.h"
#include <iostream>

using namespace std;
using namespace uhh2;


#define MYSUBJETTAGLOOP(func)\
func(btag_combinedSecondaryVertex)\
func(btag_combinedSecondaryVertexMVA)\
func(btag_DeepCSV)\
func(btag_DeepFlavour_bb)\
func(btag_DeepFlavour_b)\
func(btag_DeepFlavour_lepb)\
func(btag_DeepFlavour_uds)\
func(btag_DeepFlavour_g)\
func(btag_DeepFlavour_c)\
func(btag_DeepJet)\

#define SUBJETSBOOK(mytag)\
isLong = MyString(#mytag).find("BoostedDoubleSecondary")!=std::string::npos;\
book_TH1F("H_"+MyString(#mytag)+"_subjet1", MyString(#mytag)+"^{subjet1}", isLong? 202: 101, isLong? -1.01: -0.01, isLong? 1.01: 1.01);\
book_TH1F("H_"+MyString(#mytag)+"_subjet2", MyString(#mytag)+"^{subjet2}", isLong? 202: 101, isLong? -1.01: -0.01, isLong? 1.01: 1.01);\

#define SUBJETSFILL(mytag)\
if (cand.H().subjets().size()>0) fill_H1("H_"+MyString(#mytag)+"_subjet1", cand.H().subjets().at(0).mytag(), weight);\
if (cand.H().subjets().size()>1) fill_H1("H_"+MyString(#mytag)+"_subjet2", cand.H().subjets().at(1).mytag(), weight);\


HiggsToWWHists::HiggsToWWHists(Context& ctx, const string& dname, const string& condMatch_, const string & condMatchStatus_): HistsBase(ctx, dname), condMatch(condMatch_), condMatchStatus(condMatchStatus_) {

  h_ZprimeCandidates = ctx.get_handle<vector<ZprimeCandidate>>("ZprimeCandidate");
  h_HDecay = ctx.get_handle<float>("HDecay");
  h_ZDecay = ctx.get_handle<float>("ZDecay");
  h_ZprimeDecay = ctx.get_handle<float>("ZprimeDecay");

  massType = "m"; massPlotName = "mass";
  if (string2bool(ctx.get("invisiblechannel"))){
    massType =  "m_T";
    massPlotName = "mass_transversal";
  }

  // book all histograms here
  book_TH1F("sum_event_weights", "counting experiment", 1, 0.5, 1.5);
  book_TH1F("sum_event_weights_HtoWW", "counting experiment", 1, 0.5, 1.5);
  book_TH1F("sum_event_weights_Htobb", "counting experiment", 1, 0.5, 1.5);
  book_TH1F("sum_event_weights_Zee", "counting experiment", 1, 0.5, 1.5);
  book_TH1F("sum_event_weights_Zmumu", "counting experiment", 1, 0.5, 1.5);
  book_TH1F("sum_event_weights_Zelse", "counting experiment", 1, 0.5, 1.5);
  book_TH1F("sum_event_weights_Helse", "counting experiment", 1, 0.5, 1.5);
  book_TH1F("sum_event_weights_ZeeHtoWW", "counting experiment", 1, 0.5, 1.5);
  book_TH1F("sum_event_weights_ZeeHtobb", "counting experiment", 1, 0.5, 1.5);
  book_TH1F("sum_event_weights_ZeeHelse", "counting experiment", 1, 0.5, 1.5);
  book_TH1F("sum_event_weights_ZmumuHtoWW", "counting experiment", 1, 0.5, 1.5);
  book_TH1F("sum_event_weights_ZmumuHtobb", "counting experiment", 1, 0.5, 1.5);
  book_TH1F("sum_event_weights_ZmumuHelse", "counting experiment", 1, 0.5, 1.5);
  book_TH1F("sum_event_weights_ZelseHtoWW", "counting experiment", 1, 0.5, 1.5);
  book_TH1F("sum_event_weights_ZelseHtobb", "counting experiment", 1, 0.5, 1.5);
  book_TH1F("sum_event_weights_ZelseHelse", "counting experiment", 1, 0.5, 1.5);
  book_TH1F("Zprime_number",  "number of Zprime",                21, -.5, 20.5);

  // Zprime reconstruction
  vector<float> bins_Zprime_rebin_full;
  for (float i = 0; i <= 10000; i+=100) bins_Zprime_rebin_full.push_back(i);
  vector<float> bins_Zprime_rebin1;
  for (float i = 0; i <= 2000; i+=100) bins_Zprime_rebin1.push_back(i);
  for (float i = 2500; i <= 3000; i+=500) bins_Zprime_rebin1.push_back(i);
  bins_Zprime_rebin1.push_back(10000);

  book_TH1F("Zprime_"+massPlotName+"_rebin_full", massType + "^{Zprime} [GeV/c^{2}]", bins_Zprime_rebin_full.size()-1, &bins_Zprime_rebin_full[0]);
  book_TH1F("Zprime_"+massPlotName+"_rebin1", massType + "^{Zprime} [GeV/c^{2}]", bins_Zprime_rebin1.size()-1, &bins_Zprime_rebin1[0]);
  book_TH1F("Zprime_"+massPlotName+"_rebin2", massType + "^{Zprime} [GeV/c^{2}]", 10000, 0, 10000);
  book_TH1F("Zprime_"+massPlotName+"_rebin30",massType + "^{Zprime} [GeV/c^{2}]", 330, 0, 9900);

  for (const string & name: {"Zprime","Z","H"}) {
    // bool check = (name.compare("Zprime")) == 0;
    // book_TH1F(name+"_mass",    "m^"       +name+" [GeV/c^{2}]", check?300:40, check?0:70,check? 3000:110);
    if (name=="Zprime")   book_TH1F(name+"_"+massPlotName, massType + "^"+name+" [GeV/c^{2}]", 300,  0, 3000);
    else if (name=="Z")   book_TH1F(name+"_mass", "m^"+name+" [GeV/c^{2}]", 40,  70, 110);
    else if ((name=="H")) book_TH1F(name+"_mass", "m^"+name+" [GeV/c^{2}]", 10,   0, 200);
    if ((name=="H")) book_TH1F(name+"_mass1GeV",  "m^"+name+" [GeV/c^{2}]", 200,  0, 200);
    book_TH1F(name+"_pt",      "p_{T}^"   +name+" [GeV]",       bins_Zprime_rebin_full.size()-1, &bins_Zprime_rebin_full[0]);
    book_TH1F(name+"_energy",  "energy^"  +name+" [GeV]",       bins_Zprime_rebin1.size()-1, &bins_Zprime_rebin1[0]);
    // book_TH1F(name+"_energy",  "energy^"  +name+" [GeV]",       1000,0,10000);
    book_TH1F(name+"_eta",     "#eta"     +name,                100,-5,5);
    book_TH1F(name+"_phi",     "#phi"     +name,                50,-M_PI,M_PI);
  }

  book_TH1F("delta_phi_H_Z","#Delta#phi(H,Z)",50,0,M_PI);

  for (std::string & disc : discriminators) {
    if (FindInString("tau", disc)) {
      book_TH1F("H_"+disc,"#"+disc+"^{H}",101,0,1.01);
      book_TH1F("H_"+disc+"_rebin","#"+disc+"^{H}",30,0,1.02);
      book_TH2F("Zprime"+massPlotName+"vs"+disc, ";"+massType+"^{Zprime} [GeV/c^{2}];"+disc, 330, 0, 9900, 30,0,1.02 );
    }
    else if (FindInString("btag", disc) || FindInString("NN", disc) || FindInString("DCL", disc) ) {
      book_TH1F("H_"+disc, disc+"^{H}",101,0,1.01);
      book_TH1F("H_"+disc+"_rebin", disc+"^{H}",30,0,1.02);
      book_TH2F("Zprime"+massPlotName+"vs"+disc, ";"+massType+"^{Zprime} [GeV/c^{2}];"+disc, 330, 0, 9900, 30,0,1.02 );
    }
    else if (FindInString("chi2", disc)) book_TH1F("H_"+disc, disc+"^{H}",35,0,70);
    else book_TH1F("H_"+disc, disc+"^{H} [GeV/c^{2}]",100,0,300);
  }

  book_TH1F("H_Match","Match^{H}",17, 0, 17);
  book_TH1F("H_MatchingStatus","MatchingStatus^{H}",10, 0, 10);
  book_TH2F("H_MatchvsH_MatchingStatus",";Match^{H};MatchingStatus^{H}",17, 0, 17, 10, 0, 10);
  for (int i=1;i<18;i++) {
    H1("H_Match")->GetXaxis()->SetBinLabel(i,MatchingToString(i-1).c_str());
    H2("H_MatchvsH_MatchingStatus")->GetXaxis()->SetBinLabel(i,MatchingToString(i-1).c_str());
  }
  for (int i=1;i<11;i++) {
    H1("H_MatchingStatus")->GetXaxis()->SetBinLabel(i,MatchingStatusToString(i-1).c_str());
    H2("H_MatchvsH_MatchingStatus")->GetYaxis()->SetBinLabel(i,MatchingStatusToString(i-1).c_str());
  }

  book_TH1F("H_NN_HWWvsQCD","NN_{HWWvsQCD}^{H}",100,0,1.01);
  book_TH1F("H_NN_HWWvsQCD_1","NN_{HWWvsQCD_1}^{H}",100,0,1.01);
  book_TH1F("H_NN_HWWvsQCD_2","NN_{HWWvsQCD_2}^{H}",100,0,1.01);

  book_TH1F("btags_DeepCSV","btags_DeepCSV", 4, 0, 4);
  book_TH2F("nsubjet_btags_DeepCSV",";DeepCSV^{WP,H}_{subjet1};DeepCSV^{WP,H}_{subjet2}",4, 0, 4, 4, 0, 4);
  // book_TH2F("nsubjet_btags_DeepCSV",";nsubjet^{H};btags_DeepCSV^{H}",41, -.5, 40.5, 4, 0, 4);
  H1("btags_DeepCSV")->GetXaxis()->SetBinLabel(1,"no b-tag");
  H1("btags_DeepCSV")->GetXaxis()->SetBinLabel(2,"loose");
  H1("btags_DeepCSV")->GetXaxis()->SetBinLabel(3,"medium");
  H1("btags_DeepCSV")->GetXaxis()->SetBinLabel(4,"tight");
  H2("nsubjet_btags_DeepCSV")->GetXaxis()->SetBinLabel(1,"no b-tag");
  H2("nsubjet_btags_DeepCSV")->GetXaxis()->SetBinLabel(2,"loose");
  H2("nsubjet_btags_DeepCSV")->GetXaxis()->SetBinLabel(3,"medium");
  H2("nsubjet_btags_DeepCSV")->GetXaxis()->SetBinLabel(4,"tight");
  H2("nsubjet_btags_DeepCSV")->GetYaxis()->SetBinLabel(1,"no b-tag");
  H2("nsubjet_btags_DeepCSV")->GetYaxis()->SetBinLabel(2,"loose");
  H2("nsubjet_btags_DeepCSV")->GetYaxis()->SetBinLabel(3,"medium");
  H2("nsubjet_btags_DeepCSV")->GetYaxis()->SetBinLabel(4,"tight");

  book_TH1F("Zprime_ptinv"+massPlotName,  "p_{T}^{ll}/"+massType+"(jet,ll)",         200, 0, 2);
  book_TH1F("HT_event", "HT_event", 300, 0, 3000);
  book_TH1F("ST_event", "ST_event", 300, 0, 3000);
  book_TH1F("HT_Zprime", "HT_Zprime", 300, 0, 3000);
  book_TH1F("ST_Zprime", "ST_Zprime", 300, 0, 3000);

  book_TH2F("ST_ZprimevsZprime"+massPlotName,      ";ST_Zprime;"+massType+"^{Zprime} [GeV/c^{2}]", 300, 0, 3000,330, 0, 9900);
  book_TH2F("ST_ZprimevsDeepBoosted",              ";ST_Zprime;btag_DeepBoosted_H4qvsQCD", 300, 0, 3000, 30,0,1.02 );

  bool isLong;
  MYSUBJETTAGLOOP(SUBJETSBOOK)

}


void HiggsToWWHists::fill(const Event & event){

  auto weight = event.weight;
  fill_H1("sum_event_weights", 1., weight);

  if (event.is_valid(h_HDecay) && event.is_valid(h_ZprimeDecay)) {
    ZprimeDecay HDec = static_cast<ZprimeDecay>(int(event.get(h_HDecay)));
    ZprimeDecay ZDec = static_cast<ZprimeDecay>(int(event.get(h_ZDecay)));
    ZprimeDecay ZprimeDec = static_cast<ZprimeDecay>(int(event.get(h_ZprimeDecay)));

    if(HDec==HWW)   fill_H1("sum_event_weights_HtoWW", 1., weight);
    if(HDec==Hbb)   fill_H1("sum_event_weights_Htobb", 1., weight);
    if(HDec==Helse) fill_H1("sum_event_weights_Helse", 1., weight);
    if(ZDec==Zee)   fill_H1("sum_event_weights_Zee", 1., weight);
    if(ZDec==Zmumu) fill_H1("sum_event_weights_Zmumu", 1., weight);
    if(ZDec==Zelse) fill_H1("sum_event_weights_Zelse", 1., weight);

    if(ZprimeDec==ZeeHWW)     fill_H1("sum_event_weights_ZeeHtoWW",   1., weight);
    if(ZprimeDec==ZeeHbb)     fill_H1("sum_event_weights_ZeeHtobb",   1., weight);
    if(ZprimeDec==ZeeHelse)   fill_H1("sum_event_weights_ZeeHelse",   1., weight);
    if(ZprimeDec==ZmumuHWW)   fill_H1("sum_event_weights_ZmumuHtoWW", 1., weight);
    if(ZprimeDec==ZmumuHbb)   fill_H1("sum_event_weights_ZmumuHtobb", 1., weight);
    if(ZprimeDec==ZmumuHelse) fill_H1("sum_event_weights_ZmumuHelse", 1., weight);
    if(ZprimeDec==ZelseHWW)   fill_H1("sum_event_weights_ZelseHtoWW", 1., weight);
    if(ZprimeDec==ZelseHbb)   fill_H1("sum_event_weights_ZelseHtobb", 1., weight);
    if(ZprimeDec==ZelseHelse) fill_H1("sum_event_weights_ZelseHelse", 1., weight);
  }

  if (! event.is_valid(h_ZprimeCandidates)) return;

  vector<ZprimeCandidate> ZprimeCandidates = event.get(h_ZprimeCandidates);

  // fill the histograms.

  fill_H1("Zprime_number",  ZprimeCandidates.size(),weight);

  std::string match, matchstatus;

  for (const ZprimeCandidate & cand: ZprimeCandidates) {
    match = cand.has_discriminator("Match")? MatchingToString(FloatToMatching(cand.discriminator("Match"))) : MatchingToString(0);
    matchstatus = cand.has_discriminator("MatchingStatus")? MatchingStatusToString(FloatToMatching(cand.discriminator("MatchingStatus"))) : MatchingStatusToString(0);

    if (condMatch!="") {
      if (condMatch=="else") {
        if (match!="HWWMatch" && match!="HbbMatch" && match!="HZZMatch") continue;
      } else if (condMatch!=match) continue;
    }
    if (condMatchStatus!="" && condMatchStatus!=matchstatus) continue;

    fill_H1("Zprime_"+massPlotName,               cand.Zprime_mass(),weight);
    fill_H1("Zprime_"+massPlotName+"_rebin_full", cand.Zprime_mass(),weight);
    fill_H1("Zprime_"+massPlotName+"_rebin1",     cand.Zprime_mass(),weight);
    fill_H1("Zprime_"+massPlotName+"_rebin2",     cand.Zprime_mass(),weight);
    fill_H1("Zprime_"+massPlotName+"_rebin30",    cand.Zprime_mass(),weight);
    fill_H1("Zprime_pt",              cand.pt(),weight);
    fill_H1("Zprime_energy",          cand.energy(),weight);
    fill_H1("Zprime_eta",             cand.eta(),weight);
    fill_H1("Zprime_phi",             cand.phi(),weight);

    fill_H1("Z_mass",     cand.Z().v4().M(),weight);
    fill_H1("Z_pt",       cand.Z().pt(),weight);
    fill_H1("Z_energy",   cand.Z().energy(),weight);
    fill_H1("Z_eta",      cand.Z().eta(),weight);
    fill_H1("Z_phi",      cand.Z().phi(),weight);

    fill_H1("H_mass",     cand.H().v4().M(),weight);
    fill_H1("H_mass1GeV", cand.H().v4().M(),weight);
    fill_H1("H_pt",       cand.H().pt(),weight);
    fill_H1("H_energy",   cand.H().energy(),weight);
    fill_H1("H_eta",      cand.H().eta(),weight);
    fill_H1("H_phi",      cand.H().phi(),weight);

    double delta_phi_H_Z = fabs(deltaPhi(cand.H(), cand.Z()));
    fill_H1("delta_phi_H_Z", delta_phi_H_Z , weight);

    if (cand.discriminator("btag_DeepCSV_tight"))  H1("btags_DeepCSV")->Fill("tight",  weight);
    else if (cand.discriminator("btag_DeepCSV_medium")) H1("btags_DeepCSV")->Fill("medium", weight);
    else if (cand.discriminator("btag_DeepCSV_loose"))  H1("btags_DeepCSV")->Fill("loose",  weight);
    else H1("btags_DeepCSV")->Fill("no b-tag",  weight);

    std::vector<std::string> index_tag(2,"no b-tag");
    for (int sj_ind = 0; sj_ind <  (int)cand.discriminator("subjets"); sj_ind++) {
      if (sj_ind>1) continue;
      if ((bool)cand.discriminator("btag_DeepCSV_tight_subjet_"+std::to_string(sj_ind))) index_tag[sj_ind] = "tight";
      else if ((bool)cand.discriminator("btag_DeepCSV_medium_subjet_"+std::to_string(sj_ind))) index_tag[sj_ind] = "medium";
      else if ((bool)cand.discriminator("btag_DeepCSV_loose_subjet_"+std::to_string(sj_ind))) index_tag[sj_ind] = "loose";
      else index_tag[sj_ind] = "no b-tag";
    }
    H2("nsubjet_btags_DeepCSV")->Fill(index_tag[0].c_str(), index_tag[1].c_str(), weight);

    fill_H1("Zprime_ptinv"+massPlotName,  cand.Z().pt()/cand.Zprime_mass(),weight);

    for (std::string & disc : discriminators) {
      fill_H1("H_"+disc, cand.has_discriminator(disc)? cand.discriminator(disc): 9999 ,weight);
      if (FindInString("chi2", disc) || FindInString("SDmass", disc)) continue;
      fill_H1("H_"+disc+"_rebin", cand.has_discriminator(disc)? cand.discriminator(disc): 9999 ,weight);
      H2("Zprime"+massPlotName+"vs"+disc)->Fill(cand.Zprime_mass(), cand.has_discriminator(disc)? cand.discriminator(disc): 9999, weight);
    }

    fill_H1("H_NN_HWWvsQCD", (cand.has_discriminator("NN_HWW")&&cand.has_discriminator("NN_QCD"))? cand.discriminator("NN_HWW")/(cand.discriminator("NN_HWW")+cand.discriminator("NN_QCD")): 9999 ,weight);
    fill_H1("H_NN_HWWvsQCD_1", (cand.has_discriminator("NN_HWW_1")&&cand.has_discriminator("NN_QCD_1"))? cand.discriminator("NN_HWW_1")/(cand.discriminator("NN_HWW_1")+cand.discriminator("NN_QCD")): 9999 ,weight);
    fill_H1("H_NN_HWWvsQCD_2", (cand.has_discriminator("NN_HWW_2")&&cand.has_discriminator("NN_QCD_2"))? cand.discriminator("NN_HWW_2")/(cand.discriminator("NN_HWW_2")+cand.discriminator("NN_QCD")): 9999 ,weight);
    H1("H_Match")->Fill(match.c_str(), weight);
    H1("H_MatchingStatus")->Fill(matchstatus.c_str(), weight);
    H2("H_MatchvsH_MatchingStatus")->Fill(match.c_str(), matchstatus.c_str(), weight);

    double HT = cand.H().pt();
    double ST = cand.H().pt()+cand.Z().pt();
    fill_H1("HT_Zprime", HT, weight);
    fill_H1("ST_Zprime", ST, weight);
    H2("ST_ZprimevsZprime"+massPlotName)->Fill( ST, cand.Zprime_mass(), weight);
    H2("ST_ZprimevsDeepBoosted")->Fill(ST, cand.has_discriminator("btag_DeepBoosted_H4qvsQCD")? cand.discriminator("btag_DeepBoosted_H4qvsQCD"): 9999, weight);

    MYSUBJETTAGLOOP(SUBJETSFILL)
  }

  double HT = 0;
  // vector<std::vector<TopJet>> topjets = event.get(h_topjets);
  for (const auto & jet : *event.toppuppijets ) HT += jet.pt();
  fill_H1("HT_event", HT, weight);
  double ST = HT;
  for (const auto & electron : *event.electrons ) ST += electron.pt();
  for (const auto & muon : *event.muons ) ST += muon.pt();
  fill_H1("ST_event", ST, weight);
}

HiggsToWWHists::~HiggsToWWHists(){}


/*
#  ######  #### ##     ## ########  ##       ########    ########  #######  ########      ######  ##    ##  ######  ########
# ##    ##  ##  ###   ### ##     ## ##       ##          ##       ##     ## ##     ##    ##    ##  ##  ##  ##    ##    ##
# ##        ##  #### #### ##     ## ##       ##          ##       ##     ## ##     ##    ##         ####   ##          ##
#  ######   ##  ## ### ## ########  ##       ######      ######   ##     ## ########      ######     ##     ######     ##
#       ##  ##  ##     ## ##        ##       ##          ##       ##     ## ##   ##            ##    ##          ##    ##
# ##    ##  ##  ##     ## ##        ##       ##          ##       ##     ## ##    ##     ##    ##    ##    ##    ##    ##
#  ######  #### ##     ## ##        ######## ########    ##        #######  ##     ##     ######     ##     ######     ##
*/



HiggsToWWHistsSlim::HiggsToWWHistsSlim(Context& ctx, const string& dname, const string& condMatch_, const string & condMatchStatus_): HistsBase(ctx, dname), condMatch(condMatch_), condMatchStatus(condMatchStatus_) {

  h_ZprimeCandidates = ctx.get_handle<vector<ZprimeCandidate>>("ZprimeCandidate");

  massType = "m"; massPlotName = "mass";
  if (string2bool(ctx.get("invisiblechannel"))){
    massType =  "m_T";
    massPlotName = "mass_transversal";
  }
  // book all histograms here
  book_TH1F("Zprime_"+massPlotName+"_rebin30",massType + "^{Zprime} [GeV/c^{2}]", 330, 0, 9900);

}


void HiggsToWWHistsSlim::fill(const Event & event){

  auto weight = event.weight;
  if (! event.is_valid(h_ZprimeCandidates)) return;

  vector<ZprimeCandidate> ZprimeCandidates = event.get(h_ZprimeCandidates);

  // fill the histograms.
  std::string match, matchstatus;

  for (const ZprimeCandidate & cand: ZprimeCandidates) {
    match = cand.has_discriminator("Match")? MatchingToString(FloatToMatching(cand.discriminator("Match"))) : MatchingToString(0);
    matchstatus = cand.has_discriminator("MatchingStatus")? MatchingStatusToString(FloatToMatching(cand.discriminator("MatchingStatus"))) : MatchingStatusToString(0);

    if (condMatch!="") {
      if (condMatch=="else") {
        if (match!="HWWMatch" && match!="HbbMatch" && match!="HZZMatch") continue;
      } else if (condMatch!=match) continue;
    }
    if (condMatchStatus!="" && condMatchStatus!=matchstatus) continue;
    fill_H1("Zprime_"+massPlotName+"_rebin30",    cand.Zprime_mass(),weight);

  }

}

HiggsToWWHistsSlim::~HiggsToWWHistsSlim(){}
