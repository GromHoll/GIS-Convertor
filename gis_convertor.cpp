#include "gis_convertor.h"

/* GisHeader */
GisHeader::GisHeader() {
    defaultInit();
}

GisHeader::GisHeader(int xSize, int ySize,
                     double minX, double maxX,
                     double minY, double maxY,
                     double minZ, double maxZ) {

    defaultInit();

    calcXRange(xSize, minX, maxX);
    calcYRange(ySize, minY, maxY);

    this->minZ = minZ;
    this->maxZ = maxZ;
}

int GisHeader::getXSize() {
    return xSize;
}

int GisHeader::getYSize() {
    return ySize;
}

double GisHeader::getMinX() {
    return minX;
}

double GisHeader::getMinY() {
    return minY;
}

double GisHeader::getRoundedCellSize() {
    return (xSize > ySize) ? xCellSize : yCellSize;
}

double GisHeader::getNoDataValue() {
    return noDataValue;
}

void GisHeader::defaultInit() {
    xSize = ySize = 0;
    minX = maxX = 0;
    minY = maxY = 0;
    minZ = maxZ = 0;
    xCellSize = yCellSize = 0;
    noDataValue = ARC_DEFAULT_NO_DATA_VALUE;     
}

void GisHeader::calcXRange(int xSize, double xMin, double xMax) {
    this->xSize = xSize;
    this->minX = xMin;
    this->maxX = xMax;
    this->xCellSize = (xMax - xMin)/xSize;
}

void GisHeader::calcYRange(int ySize, double yMin, double yMax) {
    this->ySize = ySize;
    this->minY = yMin;
    this->maxY = yMax;
    this->yCellSize = (yMax - yMin)/ySize;
}

/* GisSource */
GisSource::GisSource(std::istream * in) {
    this->in = in;
}

GisHeader * GisSource::getHeader() {
    return header;    
}

GisSource::~GisSource() {
    delete header;
}
/* GrdGisSource */
GrdGisSource::GrdGisSource(std::istream * in) : GisSource(in) {
    readHeader();
}

double GrdGisSource::getNextValue() {
    return readNextValue();
}

void GrdGisSource::readHeader() {
    std::string temp;
    int xSize, ySize;
    double xMin, xMax;
    double yMin, yMax;
    double zMin, zMax;

    (*in) >> temp;   /* DSAA header line */
    (*in) >> xSize >> ySize;
    (*in) >> xMin  >> xMax;
    (*in) >> yMin  >> yMax;
    (*in) >> zMin  >> zMax;
    
    header = new GisHeader(xSize, ySize, xMin, xMax,
                           yMin, yMax, zMin, zMax);
}

double GrdGisSource::readNextValue() {
    double temp = 0;
    if ((*in) >> temp) {
       in->clear(); 
    }
    return -temp;
}

/* GisTarget */
GisTarget::GisTarget(std::ostream * out) {
    this->out = out;    
}

/* ArcGisTarget */
ArcGisTarget::ArcGisTarget(std::ostream * out) : GisTarget(out) {
    xPos, yPos = 0;
    xMax, yMax = 0;        
}

void ArcGisTarget::writeHeader(GisHeader * header) {
    (*out) << "ncols " << header->getXSize() << std::endl;
    (*out) << "nrows " << header->getYSize() << std::endl;
    (*out) << "xllcorner " << header->getMinX() << std::endl;
    (*out) << "yllcorner " << header->getMinY() << std::endl;
    (*out) << "cellsize " << header->getRoundedCellSize() << std::endl;
    (*out) << "NODATA_value " << header->getNoDataValue() << std::endl;

    xMax = header->getXSize();
    yMax = header->getYSize();
}

void ArcGisTarget::writeNextValue(double value) {
    if(yPos == yMax) {
        return;
    }
    
    if(xPos != 0) {
        (*out) << " ";    
    }
    (*out) << value; 

    if(++xPos == xMax) {
        yPos++;
        xPos = 0;
        (*out) << std::endl; 
    }            
}

/* GisConvertor */
GisConvertor::GisConvertor(GisSource * gs, GisTarget * gt) {
    this->gs = gs;
    this->gt = gt;
}

void GisConvertor::convert() {
    GisHeader * gh = gs->getHeader();
    gt->writeHeader(gh);

    for(int j = 0; j < gh->getYSize(); j++) {
        for(int i = 0; i < gh->getXSize(); i++) {
            gt->writeNextValue(gs->getNextValue());
        }
    }
}