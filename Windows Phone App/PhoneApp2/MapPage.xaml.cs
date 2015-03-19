using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using System.Device.Location;
using Windows.Devices.Geolocation;
using System.Windows.Shapes;
using Windows.UI;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using Microsoft.Phone.Maps;
using Microsoft.Phone.Maps.Controls;
using Microsoft.Phone.Maps.Toolkit;
using System.Text;
using Microsoft.Phone.Maps.Services;
using CustomePushPinToolTip;
using System.Diagnostics;

namespace PhoneApp2
{
    public partial class MapPage : PhoneApplicationPage
    {
        GeoCoordinate currentLocation = null;

        List<LocationReadings> locations = new List<LocationReadings>();

        public MapPage()
        {
            try { 
                InitializeComponent();
                locations.Add(new LocationReadings(1, 43.85575, 18.40699, 25, 10, 0.2));
                locations.Add(new LocationReadings(2, 43.85395, 18.39184, 27, 12, 0.35));
                locations.Add(new LocationReadings(3, 43.85143, 18.39832, 28, 15, 0.55));
            }
            catch (Exception ex)
            {
                string e = ex.Message;
            }
        }

        private void MapControl_Loaded(object sender, RoutedEventArgs e)
        {
            try
            {
                MapsSettings.ApplicationContext.ApplicationId = "<applicationid>";
                MapsSettings.ApplicationContext.AuthenticationToken = "<authenticationtoken>";

                MapLayer markersLayer = new MapLayer();

                Pushpin marker = new Pushpin();

                for (int i = 0; i < locations.Count; i++)
                {
                    marker = new Pushpin();
                    marker.GeoCoordinate = new GeoCoordinate(locations[i].Latitude, locations[i].Longitude);
                    marker.Style = this.Resources["PushpinStyle"] as Style;
                    MapOverlay mapOverlay = new MapOverlay();
                    mapOverlay.Content = marker;
                    mapOverlay.GeoCoordinate = new GeoCoordinate(locations[i].Latitude, locations[i].Longitude);
                    markersLayer.Add(mapOverlay);
                }

                MapControl.Layers.Add(markersLayer);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }




        private void GetLocationName(GeoCoordinate LocationsData)
        {
            LocationsData = new GeoCoordinate(43, 18);

            // Get location name
            ReverseGeocodeQuery Query = new ReverseGeocodeQuery()
            {
                GeoCoordinate = new GeoCoordinate(LocationsData.Latitude, LocationsData.Longitude)
            };
            Query.QueryCompleted += Query_QueryCompleted;
            Query.QueryAsync();
        }

        void Query_QueryCompleted(object sender, QueryCompletedEventArgs<IList<MapLocation>> e)
        {
            StringBuilder _description = new StringBuilder();
            foreach (var item in e.Result)
            {
                if (!(item.Information.Address.Street == ""))
                {
                    _description.Append(item.Information.Address.Street + ", ");

                }
           
                if (!(item.Information.Address.City == ""))
                {
                    _description.Append(item.Information.Address.City + ", ");

                }
             
                if (!(item.Information.Address.Country == ""))
                {
                    _description.Append(item.Information.Address.Country + ", ");

                }

                break;
            }
        }

        // Get current location
        private async void GetLocation()
        {
            Geolocator myGeolocator = new Geolocator();
            Geoposition myGeoposition = await myGeolocator.GetGeopositionAsync(maximumAge: TimeSpan.FromMinutes(5), timeout: TimeSpan.FromSeconds(10));
            Geocoordinate myGeocoordinate = myGeoposition.Coordinate;
            currentLocation = CoordinateConverter.ConvertGeocoordinate(myGeocoordinate);
        }
    }
}