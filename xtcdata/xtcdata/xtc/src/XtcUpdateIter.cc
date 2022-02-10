
#include "xtcdata/xtc/XtcUpdateIter.hh"
#include <sys/time.h>

using namespace XtcData;
using std::string;

template<typename T> static void _dump(const char* name,  Array<T> arrT, unsigned numWords, unsigned* shape, unsigned rank, const char* fmt)
{
    printf("'%s' ", name);
    printf(" numWords:%u rank:%u ", numWords, rank); 
    printf("(shape:");
    for (unsigned w = 0; w < rank; w++) printf(" %d",shape[w]);
    printf("): ");
    for (unsigned w = 0; w < numWords; ++w) {
        printf(fmt, arrT.data()[w]);
    }
    printf("\n");
}

void XtcUpdateIter::get_value(int i, Name& name, DescData& descdata){
    int data_rank = name.rank();
    int data_type = name.type();
    printf("%d: '%s' rank %d, type %d\n", i, name.name(), data_rank, data_type);

    switch(name.type()){
    case(Name::UINT8):{
        if(data_rank > 0){
            _dump<uint8_t>(name.name(), descdata.get_array<uint8_t>(i), _numWords, descdata.shape(name), name.rank(), " %u");
        }
        else{
            printf("'%s': %d\n",name.name(),descdata.get_value<uint8_t>(i));
        }
        break;
    }

    case(Name::UINT16):{
        if(data_rank > 0){
            _dump<uint16_t>(name.name(), descdata.get_array<uint16_t>(i), _numWords, descdata.shape(name), name.rank(), " %u");
        }
        else{
            printf("'%s': %d\n",name.name(),descdata.get_value<uint16_t>(i));
        }
        break;
    }

    case(Name::UINT32):{
        if(data_rank > 0){
            _dump<uint32_t>(name.name(), descdata.get_array<uint32_t>(i), _numWords, descdata.shape(name), name.rank(), " %u");
        }
        else{
            printf("'%s': %d\n",name.name(),descdata.get_value<uint32_t>(i));
        }
        break;
    }

    case(Name::UINT64):{
        if(data_rank > 0){
            _dump<uint64_t>(name.name(), descdata.get_array<uint64_t>(i), _numWords, descdata.shape(name), name.rank(), " %lu");
        }
        else{
            printf("'%s': %llu\n",name.name(),descdata.get_value<uint64_t>(i));
        }
        break;
    }

    case(Name::INT8):{
        if(data_rank > 0){
            _dump<int8_t>(name.name(), descdata.get_array<int8_t>(i), _numWords, descdata.shape(name), name.rank(), " %d");
        }
        else{
            printf("'%s': %d\n",name.name(),descdata.get_value<int8_t>(i));
        }
        break;
    }

    case(Name::INT16):{
        if(data_rank > 0){
            _dump<int16_t>(name.name(), descdata.get_array<int16_t>(i), _numWords, descdata.shape(name), name.rank(), " %d");
        }
        else{
            printf("'%s': %d\n",name.name(),descdata.get_value<int16_t>(i));
        }
        break;
    }

    case(Name::INT32):{
        if(data_rank > 0){
            _dump<int32_t>(name.name(), descdata.get_array<int32_t>(i), _numWords, descdata.shape(name), name.rank(), " %d");
        }
        else{
            printf("'%s': %d\n",name.name(),descdata.get_value<int32_t>(i));
        }
        break;
    }

    case(Name::INT64):{
        if(data_rank > 0){
            _dump<int64_t>(name.name(), descdata.get_array<int64_t>(i), _numWords, descdata.shape(name), name.rank(), " %ld");
        }
        else{
            printf("'%s': %lld\n",name.name(),descdata.get_value<int64_t>(i));
        }
        break;
    }

    case(Name::FLOAT):{
        if(data_rank > 0){
            _dump<float>(name.name(), descdata.get_array<float>(i), _numWords, descdata.shape(name), name.rank(), " %f");
        }
        else{
            printf("'%s': %f\n",name.name(),descdata.get_value<float>(i));
        }
        break;
    }

    case(Name::DOUBLE):{
        if(data_rank > 0){
            _dump<double>(name.name(), descdata.get_array<double>(i), _numWords, descdata.shape(name), name.rank(), " %f");
        }
        else{
            printf("'%s': %f\n",name.name(),descdata.get_value<double>(i));
        }
        break;
    }

    case(Name::CHARSTR):{
        if(data_rank > 0){
            Array<char> arrT = descdata.get_array<char>(i);
            printf("'%s': \"%s\"\n",name.name(),arrT.data());
        }
        else{
            printf("'%s': string with no rank?!?\n",name.name());
        }
        break;
    }

    case(Name::ENUMVAL):{
        if(data_rank > 0){
            _dump<int32_t>(name.name(), descdata.get_array<int32_t>(i), _numWords, descdata.shape(name), name.rank(), " %d");
        }
        else{
            printf("'%s': %d\n",name.name(),descdata.get_value<int32_t>(i));
        }
        break;
    }

    case(Name::ENUMDICT):{
        if(data_rank > 0){
            printf("'%s': enumdict with rank?!?\n", name.name());
        } else{
            printf("'%s': %d\n",name.name(),descdata.get_value<int32_t>(i));
        }
        break;
    }
    }
}

int XtcUpdateIter::process(Xtc* xtc)
{
    /* 
    A callback from iterate. 
    Dgrampy uses iterate to go through Names and ShapesData.
    For Names, both new and existing Names are copied to _tmpbuf.
    For ShapesData, only those that are not filtered out are
    copied to _tmpbuf. 
    */
    printf("\nC NEW XTC\n");
    switch (xtc->contains.id()) {
    case (TypeId::Parent): {
        iterate(xtc);
        break;
    }
    case (TypeId::Names): {
        Names& names = *(Names*)xtc;
        _namesLookup[names.namesId()] = NameIndex(names);
        Alg& alg = names.alg();
        printf("C TypeId::Names DetName: %s, Segment %d, DetType: %s, DetId: %s, Alg: %s, Version: 0x%6.6x, namesid: 0x%x, Names:\n",
               names.detName(), names.segment(), names.detType(), names.detId(),
               alg.name(), alg.version(), (int)names.namesId());

        for (unsigned i = 0; i < names.num(); i++) {
            Name& name = names.get(i);
            printf("Name: '%s' Type: %d Rank: %d\n",name.name(),name.type(), name.rank());
        }

        unsigned namesSize = sizeof(Names) + (names.num() * sizeof(Name)); 

        // copy Names to tmp buffer
        copy2tmpbuf((char*)xtc, sizeof(Xtc) + xtc->sizeofPayload());

        break;
    }
    case (TypeId::ShapesData): {
        ShapesData& shapesdata = *(ShapesData*)xtc;
        // lookup the index of the names we are supposed to use
        NamesId namesId = shapesdata.namesId();
        // if this is the namesId that we want (raw.fex), copy it
        
        // protect against the fact that this namesid
        // may not have a NamesLookup.  cpo thinks this
        // should be fatal, since it is a sign the xtc is "corrupted",
        // in some sense.
        if (_namesLookup.count(namesId)<=0) {
            printf("*** Corrupt xtc: namesid 0x%x not found in NamesLookup\n",(int)namesId);
            throw "invalid namesid";
            break;
        }
        DescData descdata(shapesdata, _namesLookup[namesId]);
        Names& names = descdata.nameindex().names();
        Data& data = shapesdata.data();
        
        Alg& alg = names.alg();
        printf("C TypeId::ShapesData DetName: %s, Alg: %s\n", names.detName(), alg.name());
    
        printf("Found %d names\n",names.num());
        for (unsigned i = 0; i < names.num(); i++) {
            Name& name = names.get(i);
            get_value(i, name, descdata);
        }

        // copy ShapesData to tmp buffer
        char detName[15];
        char algName[15];
        strcpy(detName, "hsd");
        strcpy(algName, "raw");
        if (strcmp(names.detName(), detName) == 0 && strcmp(alg.name(), algName)==0){
            _removed_size += sizeof(Xtc) + xtc->sizeofPayload();
        }else{
            copy2tmpbuf((char*)xtc, sizeof(Xtc) + xtc->sizeofPayload());
        }
        break;
    }
    default:
        break;
    }
    return Continue;
}

void XtcUpdateIter::copy2tmpbuf(char* in_buf, unsigned in_size){
    memcpy(_tmpbuf + _tmpbufsize, in_buf, in_size);
    _tmpbufsize += in_size;
}

void XtcUpdateIter::copy2buf(char* in_buf, unsigned in_size){
    memcpy(_buf + _bufsize, in_buf, in_size);
    _bufsize += in_size;
}

// Performs atomic copy that results in all necessary parts of
// an event being copied to the main output buffer _buf. This 
// requires `parent_d`, which can be updated after Names and
// ShapesData were copied to _tmpbuf. The `parent_d` is first
// copied followed by _tmpbuf (Names & ShapesData). The _tmpbuf
// is then cleared for next event.
void XtcUpdateIter::copy(Dgram* parent_d){
    copy2buf((char*) parent_d, sizeof(Dgram));
    copy2buf(_tmpbuf, _tmpbufsize);
    _tmpbufsize = 0;
}

void XtcUpdateIter::updateTimeStamp(Dgram& d, unsigned sec, unsigned nsec){
    d.time = TimeStamp(sec, nsec);
}

void XtcUpdateIter::addNames(Xtc& xtc, char* detName, char* detType, char* detId, 
        unsigned nodeId, unsigned namesId, unsigned segment,
        char* algName, uint8_t major, uint8_t minor, uint8_t micro,
        DataDef& datadef) 
{
    Alg alg0(algName,major,minor,micro);
    NamesId namesId0(nodeId, namesId);
    Names& names0 = *new(xtc) Names(detName, alg0, detType, detId, namesId0, segment);
    names0.add(xtc, datadef);
    _namesLookup[namesId0] = NameIndex(names0);
}

void XtcUpdateIter::createData(Xtc& xtc, unsigned nodeId, unsigned namesId) {
    NamesId namesId0(nodeId, namesId);
    _newData = std::unique_ptr<CreateData>(new CreateData{xtc, _namesLookup, namesId0});
}

void XtcUpdateIter::setString(char* data, DataDef& datadef, char* varname){
    // TODO: Add check for newIndex >= 0
    unsigned newIndex = datadef.index(varname);
    _newData->set_string(newIndex, data);
}

void XtcUpdateIter::setValue(unsigned nodeId, unsigned namesId,
        char* data, DataDef& datadef, char* varname){
    NamesId namesId0(nodeId, namesId);
    
    // TODO: Add check for newIndex >= 0
    unsigned newIndex = datadef.index(varname);
    
    Name& name = _namesLookup[namesId0].names().get(newIndex);
    
    switch(name.type()){
    case(Name::UINT8):{
        _newData->set_value(newIndex, *(uint8_t*)data);
        break;
    }
    case(Name::UINT16):{
        _newData->set_value(newIndex, *(uint16_t*)data);
        break;
    }
    case(Name::UINT32):{
        _newData->set_value(newIndex, *(uint32_t*)data);
        break;
    }
    case(Name::UINT64):{
        _newData->set_value(newIndex, *(uint64_t*)data);
        break;
    }
    case(Name::INT8):{
        _newData->set_value(newIndex, *(int8_t*)data);
        break;
    }
    case(Name::INT16):{
        _newData->set_value(newIndex, *(int16_t*)data);
        break;
    }
    case(Name::INT32):{
        _newData->set_value(newIndex, *(int32_t*)data);
        break;
    }
    case(Name::INT64):{
        _newData->set_value(newIndex, *(int64_t*)data);
        break;
    }
    case(Name::FLOAT):{
        _newData->set_value(newIndex, *(float*)data);
        break;
    }
    case(Name::DOUBLE):{
        _newData->set_value(newIndex, *(double*)data);
        break;
    }
    }
}

int XtcUpdateIter::getElementSize(unsigned nodeId, unsigned namesId, 
        DataDef& datadef, char* varname) {
    NamesId namesId0(nodeId, namesId);
    // TODO: Add check for newIndex >= 0
    unsigned newIndex = datadef.index(varname);
    Name& name = _namesLookup[namesId0].names().get(newIndex);
    return Name::get_element_size(name.type());
}

void XtcUpdateIter::addData(unsigned nodeId, unsigned namesId,
        unsigned* shape, char* data, DataDef& datadef, char* varname) {
    printf("addData ");
    for(unsigned i=0; i<MaxRank; i++){
        printf("shape[%u]: %u ", i, shape[i]);
    }

    NamesId namesId0(nodeId, namesId);

    // TODO: Add check for newIndex >= 0
    unsigned newIndex = datadef.index(varname);
    
    // Get shape and name (rank and type)
    Shape shp(shape);
    Name& name = _namesLookup[namesId0].names().get(newIndex);

    // Add data
    switch(name.type()){
    case(Name::UINT8):{
        Array<uint8_t> arrayT = _newData->allocate<uint8_t>(newIndex, shape);
        memcpy(arrayT.data(), (uint8_t*) data, shp.size(name));
        break;
    }
    case(Name::UINT16):{
        Array<uint16_t> arrayT = _newData->allocate<uint16_t>(newIndex, shape);
        memcpy(arrayT.data(), (uint16_t*) data, shp.size(name));
        break;
    }
    case(Name::UINT32):{
        Array<uint32_t> arrayT = _newData->allocate<uint32_t>(newIndex, shape);
        memcpy(arrayT.data(), (uint32_t*) data, shp.size(name));
        break;
    }
    case(Name::UINT64):{
        Array<uint64_t> arrayT = _newData->allocate<uint64_t>(newIndex, shape);
        memcpy(arrayT.data(), (uint64_t*) data, shp.size(name));
        break;
    }
    case(Name::INT8):{
        Array<int8_t> arrayT = _newData->allocate<int8_t>(newIndex, shape);
        memcpy(arrayT.data(), (int8_t*) data, shp.size(name));
        break;
    }
    case(Name::INT16):{
        Array<int16_t> arrayT = _newData->allocate<int16_t>(newIndex, shape);
        memcpy(arrayT.data(), (int16_t*) data, shp.size(name));
        break;
    }
    case(Name::INT32):{
        Array<int32_t> arrayT = _newData->allocate<int32_t>(newIndex, shape);
        memcpy(arrayT.data(), (int32_t*) data, shp.size(name));
        break;
    }
    case(Name::INT64):{
        Array<int64_t> arrayT = _newData->allocate<int64_t>(newIndex, shape);
        memcpy(arrayT.data(), (int64_t*) data, shp.size(name));
        break;
    }
    case(Name::FLOAT):{
        Array<float> arrayT = _newData->allocate<float>(newIndex, shape);
        memcpy(arrayT.data(), (float*) data, shp.size(name));
        break;
    }
    case(Name::DOUBLE):{
        Array<double> arrayT = _newData->allocate<double>(newIndex, shape);
        memcpy(arrayT.data(), (double*) data, shp.size(name));
        break;
    }
    case(Name::CHARSTR):{
        Array<char> arrayT = _newData->allocate<char>(newIndex, shape);
        memcpy(arrayT.data(), (char*) data, shp.size(name));
        break;
    }
    }

    printf("copied %u bytes\n", shp.size(name));
    
}

Dgram& XtcUpdateIter::createTransition(unsigned utransId, 
        bool counting_timestamps,
        unsigned timestamp_val) {
    TransitionId::Value transId = (TransitionId::Value) utransId;
    TypeId tid(TypeId::Parent, 0);
    uint64_t pulseId = 0;
    uint32_t env = 0;
    struct timeval tv;
    void* buf = malloc(BUFSIZE);
    if (counting_timestamps) {
        tv.tv_sec = 1;
        tv.tv_usec = timestamp_val;
    } else {
        gettimeofday(&tv, NULL);
    }
    Transition tr(Dgram::Event, transId, TimeStamp(tv.tv_sec, tv.tv_usec), env);
    return *new(buf) Dgram(tr, Xtc(tid));
}

