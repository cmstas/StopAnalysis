
# -*- coding: UTF-8 -*-

class E:
    """
    Properly propagates errors using all standard operations
    """
    def __init__(self, val, err=None):
        # assume poisson
        if err is None: err = abs(1.0*val)**0.5
        self.val, self.err = 1.0*val, 1.0*err

    def __add__(self, other):
        other_val, other_err = self.get_val(other)
        new_val = self.val + other_val
        new_err = (self.err**2.0 + other_err**2.0)**0.5
        return E(new_val, new_err)

    __radd__ = __add__

    def __sub__(self, other):
        other_val, other_err = self.get_val(other)
        new_val = self.val - other_val
        new_err = (self.err**2.0 + other_err**2.0)**0.5
        return E(new_val, new_err)

    def __rsub__(self, other):
        other_val, other_err = self.get_val(other)
        new_val = -(self.val - other_val)
        new_err = (self.err**2.0 + other_err**2.0)**0.5
        return E(new_val, new_err)


    def __mul__(self, other):
        other_val, other_err = self.get_val(other)
        new_val = self.val * other_val
        new_err = ((self.err * other_val)**2.0 + (other_err * self.val)**2.0)**0.5
        return E(new_val, new_err)

    __rmul__ = __mul__

    def __div__(self, other):
        other_val, other_err = self.get_val(other)
        new_val = self.val / other_val
        new_err = ((self.err/other_val)**2.0+(other_err*self.val/(other_val)**2.0)**2.0)**0.5
        return E(new_val, new_err)

    def __rdiv__(self, other):
        other_val, other_err = self.get_val(other)
        new_val = other_val / self.val
        new_err = ((other_err/self.val)**2.0+(self.err*other_val/(self.val)**2.0)**2.0)**0.5
        return E(new_val, new_err)

    def __pow__(self, other):
        # doesn't accept an argument of class E, only normal number
        new_val = self.val ** other
        new_err = ((other * self.val**(other-1) * self.err)**2.0)**0.5
        return E(new_val, new_err)

    def __neg__(self):
        return E(-1.*self.val, self.err)

    def __eq__(self, other):
        other_val, other_err = self.get_val(other)
        return self.val == other_val and self.err == other_err

    def __ne__(self, other):
        return not self.__eq__(other)

    def get_val(self, other):
        other_val, other_err = other, 0.0
        if type(other)==type(self):
            other_val, other_err = other.val, other.err
        return other_val, other_err

    def round(self, ndec):
        if ndec == 0:
            self.val = int(self.val)
        else:
            self.val = round(self.val,ndec)
        self.err = round(self.err,ndec)
        return self

    def rep(self):
        use_ascii = False
        if use_ascii:
            sep = "+-"
        else:
            sep = u"\u00B1".encode("utf-8")
        return "%s %s %s" % (str(self.val), sep, str(self.err))

    def pct_rep(self,ndec=1):
        use_ascii = False
        if use_ascii:
            sep = "+-"
        else:
            sep = u"\u00B1".encode("utf-8")
        return "(%s %s %s)%%" % (str(round(self.val*100,ndec)), sep, str(round(self.err*100,ndec)))

    __str__ = rep

    __repr__ = rep

    def __getitem__(self, idx):
        if idx==0: return self.val
        elif idx==1: return self.err
        else: raise IndexError

root_e = None
def get_significance(exp,obs):
    """
    https://root.cern.ch/root/html526/RooStats__NumberCountingUtils.html
    """
    global root_e
    if not root_e: import ROOT as root_e
    return root_e.RooStats.NumberCountingUtils.BinomialObsZ(obs[0], exp[0], exp[1]/exp[0])

def get_StoSB(S, B):
    ''' get signal significace metric S/sqrt(S+B) '''
    s, se = S.val, S.err
    b, be = B.val, B.err
    if b == 0: return E(0, 0)
    e = (s+2*b)**2 * se**2 + s**2 * be**2
    e /= 4*(s+b)**3
    return E(s/((s+b)**0.5), e**0.5)

def get_efficiency(num, den):
    ''' get efficiency with binomial error '''
    if den.val == 0: return E(0, 0)
    if num.val == den.val: return E(1,0)
    b1, b2 = num.val, den.val
    b1sq, b2sq = b1**2, b2**2
    e1sq, e2sq = num.err**2, den.err**2
    err = abs( ( (1. - 2.* b1 / b2) * e1sq  + b1sq * e2sq / b2sq ) / b2sq )
    return E(b1/b2, err)

if __name__ == "__main__":
    v1 = E(10.0,1.0)
    v2 = E(10.0,1.0)
    v3 = E(10.0,2.0)
    v4 = E(20.0,1.0)

    print v1+v2
    print v1+1.0
    print v1-v2
    print v1-1.0
    print v1/v2
    print v1*v2
    print (v1+v2)*(v1*3.0-v2)/(v1*2.0)
    print v1*1.0
    print 1.0*v1
    print 1.0+v1
    print 1.0-v1
    print 1.0/v1
    print v1/1.0
    print v1**2
    print (v1+v2)[0], (v1+v2)[1]
    print v3/v4
    val, err = v4/v3
    print get_efficiency(E(10.0), E(20.0))
    print get_StoSB(v3, v4)
