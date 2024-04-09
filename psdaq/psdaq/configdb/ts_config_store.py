from psdaq.configdb.typed_json import cdict
from psdaq.configdb.tsdef import *
import psdaq.configdb.configdb as cdb

def usual_cdict():
    top = cdict()

    top.setAlg('config', [2,0,1])

    top.set("firmwareBuild:RO"  , "-", 'CHARSTR')
    top.set("firmwareVersion:RO",   0, 'UINT32')

    top.define_enum('trigModeEnum', {key:val for val,key in enumerate(
        ['FixedRate', 'ACRate', 'EventCode', 'Sequence'])})
    top.define_enum('fixedRateEnum', fixedRateHzToMarker)
    top.define_enum('acRateEnum', acRateHzToMarker)
    top.define_enum('boolEnum', {'False':0, 'True':1})
    top.define_enum('seqEnum', {'Bursts': 15, '10KRates': 16, 'Local': 17})

    seqBurstNames = []
    for j in range(16):
        seqBurstNames.append('%dx%dns'%(2**(1+(j%4)),1080*(j/4)))

    top.define_enum('linacEnum', {'Cu': 0, 'SC': 1})
    top.define_enum('seqBurstEnum', {key:val for val,key in enumerate(seqBurstNames)})

    seqFixedRateNames = []
    for j in range(16):
        seqFixedRateNames.append('%dkHz'%((j+1)*10))

    top.define_enum('seqFixedRateEnum', {key:val for val,key in enumerate(seqFixedRateNames)})

    seqLocal = ['%u0kHz'%(4*i+4) for i in range(16)]
    top.define_enum('seqLocalEnum', {key:val for val,key in enumerate(seqLocal)})
    top.define_enum('destSelectEnum', {'Include': 0, 'DontCare': 1})

    help_str  = "-- user --"
    help_str += "\nLINAC        : Timing System source (Cu/SC)"
    help_str += "\n-- user.groupN (Cu mode) --"
    help_str += "\neventcode    : Trigger eventcode"
    help_str += "\n-- user.groupN (SC mode) --"
    help_str += "\ntrigger is a combination of rate and destn selection"
    help_str += "\nfixed.rate   : fixed period trigger rate"
    help_str += "\nac.rate      : AC power syncd trigger rate per timeslot"
    help_str += "\nac.ts[6]     : include timeslot in trigger rate"
    help_str += "\neventcode    : trigger eventcode"
    help_str += "\nseq.mode     : choice of event sequencer"
    help_str += "\nseq.channel  : choice channel within sequencer"
    help_str += "\ndestn.select : qualifier for following destn masks"
    help_str += "\n   Inclusive : trigger when beam to one of destns"
    help_str += "\n   Exclusive : trigger when not beam to one of destns"
    help_str += "\n   DontCare  : ignore destn"
    help_str += "\ndestn.destN  : add destN to trigger consideration"
    help_str += "\nrawInsertRate: raw data retention rate (Hz)" 
    top.set('help:RO', help_str, 'CHARSTR')

    top.set('user.LINAC', 0, 'linacEnum')

    for group in range(8):
        grp_prefix = 'user.Cu.group'+str(group)+'_'
        if group==6:
            top.set(grp_prefix+'eventcode', 272, 'UINT32')
        else:
            top.set(grp_prefix+'eventcode', 40, 'UINT32')

        top.set(grp_prefix+'rawInsertRate', 1., 'FLOAT')

        grp_prefix = 'user.SC.group'+str(group)+'.'
        top.set(grp_prefix+'trigMode', 0, 'trigModeEnum') # default to fixed rate
        top.set(grp_prefix+'fixed.rate', 6, 'fixedRateEnum') # default 1Hz

        top.set(grp_prefix+'ac.rate', 0, 'acRateEnum')
        for tsnum in range(6):
            top.set(grp_prefix+'ac.ts'+str(tsnum), 0, 'boolEnum')

        top.set(grp_prefix+'eventcode', 272, 'UINT32')

        top.set(grp_prefix+'seq.mode'      ,15, 'seqEnum')
        top.set(grp_prefix+'seq.burst.mode', 0, 'seqBurstEnum')
        top.set(grp_prefix+'seq.fixed.rate', 0, 'seqFixedRateEnum')
        top.set(grp_prefix+'seq.local.rate', 0, 'seqLocalEnum')

        top.set(grp_prefix+'destination.select', 1, 'destSelectEnum')
        for destnum in range(16):
            top.set(grp_prefix+'destination.dest'+str(destnum), 0, 'boolEnum')

        top.set(grp_prefix+'rawInsertRate', 1., 'FLOAT')

        grp_prefix = 'expert.group'+str(group)+'.'
        for inhnum in range(4):
            top.set(grp_prefix+'inhibit'+str(inhnum)+'.enable', 0, 'boolEnum')
            top.set(grp_prefix+'inhibit'+str(inhnum)+'.interval',1,'UINT32')
            top.set(grp_prefix+'inhibit'+str(inhnum)+'.limit',1,'UINT32')

    return top

def calib_cdict():
    top = cdict()

    top.setAlg('config', [2,0,1])

    # This configuration is respective to a configuration of the same
    # instrument/detname_seqnum and of the same version but of the following cfgType
    top.set('_cfgTypeRef', 'BEAM', 'CHARSTR')

    help_str  = "-- user --"
    help_str += "\n_cfgTypeRef\t: Unlisted parameters are inherited from this"
    help_str += "\n\t  Configuration Type"
    help_str += "\n-- user.groupN (SC mode) --"
    help_str += "\nfixed.rate\t: Fixed period trigger rate"
    top.set('help:RO', help_str, 'CHARSTR')

    top.define_enum('trigModeEnum', {key:val for val,key in enumerate(
        ['FixedRate', 'ACRate', 'EventCode', 'Sequence'])})
    top.define_enum('fixedRateEnum', fixedRateHzToMarker)

    # For now, only the ability to change the fixed-rate trigger rate is provided
    for group in range(8):
        grp_prefix = 'user.SC.group'+str(group)+'.'
        top.set(grp_prefix+'trigMode', 0, 'trigModeEnum') # default to fixed rate
        top.set(grp_prefix+'fixed.rate', 2, 'fixedRateEnum') # default 100Hz

    return top

if __name__ == "__main__":
    # these are the current default values, but I put them here to be explicit
    create = True
    dbname = 'configDB'

    args = cdb.createArgs().args
    db   = 'configdb' if args.prod else 'devconfigdb'
    url  = f'https://pswww.slac.stanford.edu/ws-auth/{db}/ws/'

    mycdb = cdb.configdb(url, args.inst, create,
                         root=dbname, user=args.user, password=args.password)
    mycdb.add_alias(args.alias)

    # this needs to be called once per detType at the
    # "beginning of time" to create the collection name (same as detType
    # in top.setInfo).  It doesn't hurt to call it again if the collection
    # already exists, however.
    mycdb.add_device_config('ts')

    if args.alias == 'CALIB':
        top = calib_cdict()
    else:
        top = usual_cdict()
    top.setInfo('ts', args.name, args.segm, args.id, 'No comment')

    mycdb.modify_device(args.alias, top)
    #mycdb.print_configs()
