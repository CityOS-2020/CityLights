using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PhoneApp2
{
    public class LocationReadings
    {
        public LocationReadings(int id, double lat, double lng, int temp, int humidity, double pollution)
        {
            ID = id;
            Latitude = lat;
            Longitude = lng;
            Temperature = temp;
            Humidity = humidity;
            Pollution = pollution;
        }

        public int ID { get; set; }
        public double Latitude { get; set; }
        public double Longitude { get; set; }
        public int Temperature { get; set; }
        public int Humidity { get; set; }
        public double Pollution { get; set; }
    }
}
