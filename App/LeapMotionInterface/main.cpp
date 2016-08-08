/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::Isosurface class.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include <kvs/Message>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/PolygonObject>
#include <kvs/Isosurface>
#include <kvs/OrthoSlice>
#include <kvs/HydrogenVolumeData>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <Leap.h>


const std::string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
class SampleListener : public Leap::Listener
{
private:

  kvs::glut::Screen& m_screen;
  kvs::StructuredVolumeObject* m_volume;


public:

  SampleListener( kvs::glut::Screen& screen, kvs::StructuredVolumeObject* volume ) : m_screen( screen ), m_volume( volume ) {}


  void onInit( const Leap::Controller& controller )
  {
    std::cout << "Init" << std::endl;
  }

  void  onConnect(const Leap::Controller& controller)
  {
    std::cout << "Connected" << std::endl;
    /*   controller.enableGesture(Leap::Gesture::TYPE_CIRCLE);
    controller.enableGesture(Leap::Gesture::TYPE_KEY_TAP);
    controller.enableGesture(Leap::Gesture::TYPE_SCREEN_TAP);
    controller.enableGesture(Leap::Gesture::TYPE_SWIPE);
    */
  }

  void onFrame( const Leap::Controller& controller )
  {
    const Leap::Frame frame = controller.frame();
    std::cout << "Frame id:" << frame.id() << std::endl;
    
   
    Leap::HandList hands = frame.hands();
    for ( Leap::HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl )
      {
	std::cout << *hl << std::endl;
	int count = frame.hands().count();
	std::cout << "count" << count << std::endl;
	const Leap::Hand hand = *hl;
	if ( hand.isLeft() )
	  {
	    std::cout << "Left" << std::endl;
	     m_screen.scene()->object("Iso")->hide();
	  }
	else if ( hand.isRight() )
	  {
	    std::cout << "Right" << std::endl;
	    m_screen.scene()->object("Iso")->show();
	    
	  }
	bool isosurface_mode = true;
	if( isosurface_mode ){
	Leap::Vector handCenter = hand.palmPosition();
	std::cout << "Right center:" << handCenter.y << std::endl;
	const float p = m_volume->resolution().z() * handCenter.y / 100;

	/*
	// Get the hand's normal vector and direction                                                                                          
	  const Leap::Vector normal = hand.palmNormal();  
	    float pitchInRadians = normal.pitch();
	*/
	const kvs::OrthoSlice::AlignedAxis a = kvs::OrthoSlice::ZAxis;

	const kvs::TransferFunction t( 256 );
	kvs::PolygonObject* object2 = new kvs::OrthoSlice( m_volume, p, a, t );
	object2->setName("Ortho");

	m_screen.scene()->replaceObject("Ortho",object2);
	/*
	  if (handCenter.z < m_volume->minValue())
	  {
                handCenter.z = m_volume->minValue();                                                                                                                                                                                                                                                                          
              }                                                                                                                                                                                                                                                                                                               
            else if (handCenter.z > m_volume->maxValue() )                                                                                                                                                                                                                                                                    
              {                                                                                                                                                                                                                                                                                                               
                handCenter.z = m_volume->maxValue();                                                                                                                                                                                                                                                                          
              }                                                                                                                                                                                                                                                                                                               
            // const double i = ( m_volume->maxValue() + m_volume->minValue() ) * handCenter.z / 100 ;                                                                                                                                                                                                                        
            const double i = handCenter.z;                                                                                                                                                                                                                                                                                    
            const kvs::PolygonObject::NormalType n = kvs::PolygonObject::VertexNormal;                                                                                                                                                                                                                                        
            const bool d = false;                                                                                                                                                                                                                                                                                             
            const kvs::TransferFunction t( 256 );                                                                                                                                                                                                                                                                             
            kvs::PolygonObject* object1 = new kvs::Isosurface( m_volume, i, n, d, t );                                                                                                                                                                                                                                        
            object1->setName("Iso");                                                                                                                                                                                                                                                                                          
            m_screen.scene()->replaceObject("Iso",object1);                                                                                                                                                                                                                                                                   
	*/
	}
	/*	const Leap::FingerList fingers = frame.hands()[0].fingers();
	for (Leap::FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); fl++){
	  const Leap::Bone bone;
	  Leap::Bone::Type boneType;
	  bonetype = stat
	  std::cout <<std::string(4, ' ') <<  fingerNames[finger.type()]
		    << std::endl;
	}
	*/
      }
    /*
    const Leap::GestureList gestures = frame.gestures();
    for(int g = 0; g < gestures.count(); ++g)
      {
	Leap::Gesture gesture= gestures[g];
	switch(gesture.type()) 
	  {
	  case  Leap::Gesture::TYPE_CIRCLE: 
	    {
	      Leap::CircleGesture circle = gesture;
	      std::cout << "Circle" << std::endl;
	      
	      //Calculate angle swept since last frame
	      float sweptAngle = 0;
	      if(circle.state() != Leap::Gesture::STATE_START)
		{
		  Leap::CircleGesture previousUpdate = Leap::CircleGesture(controller.frame(1).gesture(circle.id()));
		  sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * Leap::PI;
		}
	      std::cout << std::string(2, ' ')
			<< "Circle id: " << gesture.id()
			<< ", progress: " << circle.progress()
			<< ", angle " << sweptAngle * Leap:: RAD_TO_DEG << std::endl;
	      m_volume->updateMinMaxValues();
	      float safeAngle  = sweptAngle * 57.295779513f / Leap::PI;
	      if(safeAngle >=0.5)
		{
		  safeAngle = 0.5;
		}
	      const double i = m_volume->maxValue() * safeAngle;
	      std::cout << "shu" << safeAngle << std::endl;
	      //const double i = sweptAngle;
	      const kvs::PolygonObject::NormalType n = kvs::PolygonObject::VertexNormal;
	      const bool d = false;
	      const kvs::TransferFunction t( 256 );
	      kvs::PolygonObject* object1 = new kvs::Isosurface( m_volume, i, n, d, t );
	      object1->setName("Iso");
	      
	      m_screen.scene()->replaceObject("Iso",object1);
	      break;
	    }    
	  case Leap:: Gesture::TYPE_SWIPE: 
	    std::cout << "Swipe id: " << gesture.id() <<std::endl;
	    break;
	  case Leap::Gesture::TYPE_KEY_TAP:
	    std::cout << "Key Tap id: " << gesture.id() <<std::endl;
	    break;
	  case Leap::Gesture::TYPE_SCREEN_TAP:
	    std::cout << "Screen Type id: " << gesture.id() <<std::endl;
	    break;
	  default:
	    std::cout << std::string(2, ' ')  << "Unknown gesture type." << std::endl;
	    break;
	
	  }
      } */
 
	m_screen.redraw();

      /*
    const Leap::Frame frame = controller.frame();
    std::cout << "Frame id: " << frame.id() << std::endl;
    Leap::HandList hands = frame.hands();
    for (Leap::HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
      // Get the first hand                                                                                                                 
      const Leap::Hand hand = *hl;
      std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
      std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
		<< ", palm position: " << hand.palmPosition() << std::endl;
      // Get the hand's normal vector and direction                                                                                         
      const Leap::Vector normal = hand.palmNormal();
      const Leap::Vector direction = hand.direction();

      // Calculate the hand's pitch, roll, and yaw angles                                                                                   
      std::cout << std::string(2, ' ') <<  "pitch: " << direction.pitch() * Leap::RAD_TO_DEG << " degrees, "
		<< "roll: " << normal.roll() * Leap::RAD_TO_DEG << " degrees, "
		<< "yaw: " << direction.yaw() * Leap::RAD_TO_DEG << " degrees" << std::endl;
    */
      }
  };


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument counter
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    /* Read volume data from the specified data file. If the data file is not
     * specified, scalar hydrogen volume data is created by using
     * kvs::HydrogenVolumeData class.
     */
    kvs::StructuredVolumeObject* volume = NULL;
    if ( argc > 1 ) volume = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
    else            volume = new kvs::HydrogenVolumeData( kvs::Vector3ui( 64, 64, 64 ) );

    if ( !volume )
    {
        kvsMessageError( "Cannot create a structured volume object." );
        return( false );
    }

    /* Extract surfaces by using the Isosurface class.
     *    i: isolevel
     *    n: NormalType (PolygonNormal/VertexNormal)
     *    d: check flag whether the duplicate vertices are deleted (false) or not
     *    t: transfer function
     */
    volume->updateMinMaxValues();
    const double i = ( volume->maxValue() + volume->minValue() ) * 0.5;
    const kvs::PolygonObject::NormalType n = kvs::PolygonObject::VertexNormal;
    const bool d = false;
    const kvs::TransferFunction t( 256 );
    kvs::PolygonObject* object1 = new kvs::Isosurface( volume, i, n, d, t );
    if ( !object1 )
    {
        kvsMessageError( "Cannot create a polygon object1." );
        delete volume;
        return( false );
    }

    object1->setName("Iso");

    /* Extract orthogonal planes by using OrthoSlice class.                                                                    
     *    p: plane position.                                                                                                   
     *    a: axis of the orthogonal plane.                                                                                     
     *    t: transfer function.                                                                                                
     */
    const float p = volume->resolution().z() * 0.5f;
    const kvs::OrthoSlice::AlignedAxis a = kvs::OrthoSlice::ZAxis;
    // const kvs::TransferFunction t( 256 );
    kvs::PolygonObject* object2 = new kvs::OrthoSlice( volume, p, a, t );
    if ( !object2 )
      {
        kvsMessageError( "Cannot create a polygon object2." );
        delete volume;
        return( false );
      }

    object2->setName("Ortho");

    //    delete volume;

    // Screen.
    kvs::glut::Screen screen( &app );
    screen.registerObject( object1 );
    screen.registerObject( object2 );
    screen.setGeometry( 0, 0, 512, 512 );
    screen.setTitle( "kvs::Isosurface" );
    screen.show();


    SampleListener listener( screen, volume );
    Leap::Controller controller;
    controller.addListener( listener );


    return( app.run() );
}