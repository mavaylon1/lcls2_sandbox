#ifndef HSD_I2cSwitch_hh
#define HSD_I2cSwitch_hh

#include "psdaq/mmhw/RegProxy.hh"
#include "Globals.hh"

namespace Pds {
  namespace HSD {
    class I2cSwitch {
    public:
      enum Port { PrimaryFmc=1, SecondaryFmc=2, SFP=4, LocalBus=8 };
      void select(Port);
      void dump () const;
    private:
      Mmhw::RegProxy _control;
      uint32_t  _reserved[255];
    };
  };
};

#endif
