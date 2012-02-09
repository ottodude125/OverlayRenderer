OverlayRender is a sub-system of a 2012 Senior Design Project at the University of Massachusetts Amherst. 

The main project is NavFocus whose goal is to provide a non-distracting alternative to hand-held Global Positioning System (GPS) devices used in automobiles. 

The project has three main sub-systems. The first uses a modified version of Navit to provide maps, a user interface, and obtain data from the GPS module. The second is RoadTracking which takes video footage, analyzes it to find the boundaries of the road. The third system combines the RoadTracking data with data obtained from the GPS to render images upon the windshield. 

The OverlayRenderer subsystem is responsible for taking the aforementioned obtained data and produce the images. 

The system is broken into four parts
	a) OverlayRendererControl - Responsible for controlling the entire sub-system
	b) RouteInputAnalyzer - Analyzes new(most recent) data obtained from GPS
	c) ChoicePath - Analyzes new(most recent) data obtained from Road Tracking
	d) ImageCreation - Create guidance image based on previous analysis