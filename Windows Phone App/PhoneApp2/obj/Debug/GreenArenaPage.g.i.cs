﻿#pragma checksum "C:\Users\Haris\Desktop\PhoneApp2\PhoneApp2\GreenArenaPage.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "173C35F406373D496564C63ED030F44D"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.34209
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

using Microsoft.Phone.Controls;
using System;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Automation.Peers;
using System.Windows.Automation.Provider;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Resources;
using System.Windows.Shapes;
using System.Windows.Threading;


namespace PhoneApp2 {
    
    
    public partial class GreenArenaPage : Microsoft.Phone.Controls.PhoneApplicationPage {
        
        internal System.Windows.Controls.Grid LayoutRoot;
        
        internal System.Windows.Controls.Grid ContentPanel;
        
        internal System.Windows.Controls.TextBlock StreetName;
        
        internal System.Windows.Controls.TextBlock TemperatureValue;
        
        internal System.Windows.Controls.TextBlock HumidityValue;
        
        internal System.Windows.Controls.TextBlock PollutionValue;
        
        private bool _contentLoaded;
        
        /// <summary>
        /// InitializeComponent
        /// </summary>
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        public void InitializeComponent() {
            if (_contentLoaded) {
                return;
            }
            _contentLoaded = true;
            System.Windows.Application.LoadComponent(this, new System.Uri("/PhoneApp2;component/GreenArenaPage.xaml", System.UriKind.Relative));
            this.LayoutRoot = ((System.Windows.Controls.Grid)(this.FindName("LayoutRoot")));
            this.ContentPanel = ((System.Windows.Controls.Grid)(this.FindName("ContentPanel")));
            this.StreetName = ((System.Windows.Controls.TextBlock)(this.FindName("StreetName")));
            this.TemperatureValue = ((System.Windows.Controls.TextBlock)(this.FindName("TemperatureValue")));
            this.HumidityValue = ((System.Windows.Controls.TextBlock)(this.FindName("HumidityValue")));
            this.PollutionValue = ((System.Windows.Controls.TextBlock)(this.FindName("PollutionValue")));
        }
    }
}

