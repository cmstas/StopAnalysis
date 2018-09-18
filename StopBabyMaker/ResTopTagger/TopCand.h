#ifndef TOPCAND_H
#define TOPCAND_H

#include <vector>
#include "Math/LorentzVector.h"
#include "Math/Point3D.h"
typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

class TopCand {
 public:
  TopCand() = delete;
  TopCand(const size_t inb, const size_t inj2, const size_t inj3, const std::vector<LorentzVector>* jetp4vec)
      : disc(-1), ib_(inb), ij2_(inj2), ij3_(inj3)
  {
    if (jetp4vec->at(inj3).pt() > jetp4vec->at(inj2).pt()) {
      ij2_ = inj3; ij3_ = inj2;
    }
    wcand = jetp4vec->at(ij2_) + jetp4vec->at(ij3_);
    topcand = jetp4vec->at(ib_) + wcand;
  }

  int getIdxForb()  const { return ib_; }
  int getIdxForj2() const { return ij2_; }
  int getIdxForj3() const { return ij3_; }

  bool passMassW(double range=40)   const { return std::abs(wcand.mass()-80)    <= range; }
  bool passMassTop(double range=80) const { return std::abs(topcand.mass()-175) <= range; }
  bool sameAs(const TopCand &c) const { return ib_==c.ib_ && ij2_==c.ij2_ && ij3_==c.ij3_; }
  bool similarAs(const TopCand &c) const {
    return (ib_==c.ib_ && ij2_==c.ij2_ && ij3_==c.ij3_) || (ib_==c.ib_ && ij2_==c.ij3_ && ij3_==c.ij2_)
        || (ib_==c.ij2_ && ij2_==c.ib_ && ij3_==c.ij3_) || (ib_==c.ij2_ && ij2_==c.ij3_ && ij3_==c.ib_)
        || (ib_==c.ij3_ && ij2_==c.ib_ && ij3_==c.ij2_) || (ib_==c.ij3_ && ij2_==c.ij2_ && ij3_==c.ib_);
  }
  bool overlaps(const TopCand &c) const {
    return (ib_==c.ib_ || ib_==c.ij2_ || ib_==c.ij3_ || ij2_==c.ib_ || ij2_==c.ij2_ ||
            ij2_==c.ij3_ || ij3_==c.ib_ || ij3_==c.ij2_ || ij3_==c.ij3_);
  }

  LorentzVector wcand;
  LorentzVector topcand;
  double disc;

  friend class TopCandTree;
  friend class ResolvedTopMVA;

 private:
  size_t ib_;
  size_t ij2_;
  size_t ij3_;

};

#endif
