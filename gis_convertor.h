#ifndef GIS_CONVERTOR_H
#define GIS_CONVERTOR_H

#include <iostream>
#include <string>

#define ARC_DEFAULT_NO_DATA_VALUE -9999

class GisHeader {
    private:
        int xSize, ySize;

        double minX, maxX;
        double minY, maxY;
        double minZ, maxZ;

        double xCellSize, yCellSize;

        double noDataValue;	

    public:
        GisHeader();		
        /* Surfer GRD header style */
        GisHeader(int xSize, int ySize,
                  double minX, double maxX,
                  double minY, double maxY,
                  double minZ, double maxZ);

        int getXSize();
        int getYSize();
        double getMinX();
        double getMinY();
        double getRoundedCellSize();
        double getNoDataValue();
		
	private:
		void defaultInit();
        void calcXRange(int xSize, double xMin, double xMax);
        void calcYRange(int ySize, double yMin, double yMax);
};

class GisSource {
    protected:        
        GisHeader * header;
        std::istream * in;

    public:
        GisSource(std::istream * in);

        GisHeader * getHeader();
        
        virtual ~GisSource();

        virtual double getNextValue() = 0;
};

class GrdGisSource : public GisSource {
    public:
        GrdGisSource(std::istream * in);

        double getNextValue();
    private:
        void readHeader();
        double readNextValue();
};

class GisTarget {
    protected:
        std::ostream * out;

    public:
        GisTarget(std::ostream * out);

        virtual void writeHeader(GisHeader * header) = 0;
        virtual void writeNextValue(double value) = 0;
};

class ArcGisTarget : public GisTarget {
    private:
        int xPos, yPos;
        int xMax, yMax;

    public:
        ArcGisTarget(std::ostream * out);

        void writeHeader(GisHeader * header);
        void writeNextValue(double value);
};

class GisConvertor {
    private:
        GisSource * gs;
        GisTarget * gt;

    public:
        GisConvertor(GisSource * gs, GisTarget * gt);

        void convert();
};

#endif /* GIS_CONVERTOR_H */