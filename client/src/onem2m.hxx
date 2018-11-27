/* 
 *	SPDX-License-Identifier:	BSD-3-Clause 
 * The above license addresses copyright obligations. Please refer to Section 10.4 of the 
 * ATIS Operating Procedures for information about patent licensing obligations under 
 * ATIS' Intellectual Property Rights Policy.
 */


/** \file onem2m.hxx
 * This file contains the constants used in the OS-IoT API.
 */

/** \mainpage ATIS OS-IoT
 * \section intro_sec Introduction
 *
 * This library allows an IoT client to interact with resources stored on a oneM2M Infrastructure Node (IN) or Mobile Node (MN) via the oneM2M protocol.
 * For general information about OS-IoT refer to http://os-iot.org/.
 * For a practical introduction to oneM2M concepts and procedures see the oneM2M Developers Guide http://onem2m.org/application-developer-guide/welcome.
 *
 * \section docguide_sec Guide to the API Specification
 *
 * This detailed API specification should be read in conjunction with the OS-IoT Manual which is available from the OS-IoT
 * web site.
 *
 * The public interface to the library is contained in the namespace onem2m.
 *
 * Functions to support oneM2M network operations are documented in the onem2m functions.
 *
 * The oneM2M resource object library is documented in the onem2m classes. Note in paricular the 
 * \ref optionalsequence "special handling for optional and sequence elements".
 * For the semantics and structure of oneM2M
 * resource objects refer to the oneM2M specifications. For information about the supported resource fields
 * refer to the OS-IoT Manual.
 *
 * \section usage_sec Library Usage
 *
 * To use the library, the application should follow the following steps:
 * -# Initialize the library with onem2m::initialize()
 * -# Update any values for pre-set parameters (e.g. the 'from' parameter) to those needed for the application using functions like onem2m::setHostName(), onem2m::setFrom() or onem2m::setProtocol().
 * -# For CREATE and UPDATE operations create a oneM2M resource object of the appropriate type and populate with the data 
 * to be included in the resource. Example classes that represent oneM2M resource objects are onem2m::AE, onem2m::container,
 * onem2m::contentInstance, onem2m::subscription
 * and onem2m::accessControlPolicy.
 * -# Call a high level function such as retrieveResource(), deleteResource(), createResource(), updateResource() to perform the desired operation on resources at the IN. These functions will generate signalling to the IN and return the result to the calling application. In this version of the library these are synchronous (blocking) functions.
 * -# Repeat steps 2 and/or 3 and/or 4 as needed.
 * -# Terminate the library with onem2m::terminate()
 *
 * Refer to the file osiotcmd.cxx for an example command-line application that demonstrates this use of the API and 
 * also how to interact with oneM2M resource objects. A further example application and a guide to how to build your
 * own applications are available on the OS-IoT web site.
 *
 * Note that the function onem2m::send() is a low level routine and should only be used by advanced
 * applications that need to interact with the oneM2M signalling at a finer level of granularity than is possible with the
 * high level routines.
 */

/** \page optionalsequence Special Hanlding for _optional, _sequence and _base types
 * \section optionalseuqnceinfo Introduction
 *
 * Classes that contain parameters that are optional or sequences according ot the oneM2M XML protocol definition will
 * generate special templated types with names ending "_optional" or "_sequence" respectively. For example, 
 * the onem2m::resource class
 * contain a \ref onem2m::resource::labels "labels" element which is of type "labels_optional".
 *
 * These types provide a set of standard methods to manipulate the collection of data in the element which must be used.
 * These methods allow the _optional or _sequence element to be translated in to it's underlying type. For example,
 * if an object r is of type onem2m::resource then r.labels().get() will return an object of type onem2m::labels.
 * Note that attempting to perform get() on an optional element that is not present will result in a memory
 * violation. The present() method should be used to check if an optional element is present before accessing.
 *
 * Classes that represent lists of information may be built on top of a type with a name ending "_base". For example 
 * onem2m::labels is built on onem2m::labels_base. In this case an interface of a standard C++ sequence (e.g., std::vector) will 
 * be provided.
 * 
 * \section optionalinfo _optional type interface
 * 
 * See section 2.8.2 of the Code Synthesis C++/Tree Mapping User Manual.
 * 
 * Types named "_optional" have the following interface:
 *
 * ~~~~
 *    template <typename X>
 *    class optional
 *    {
 *    public:
 *      optional ();
 *    
 *      // Makes a deep copy.
 *      //
 *      explicit
 *      optional (const X&);
 *    
 *      // Assumes ownership.
 *      //
 *      explicit
 *      optional (std::[auto|unique]_ptr<X>);
 *    
 *      optional (const optional&);
 *    
 *    public:
 *      optional&
 *      operator= (const X&);
 *    
 *      optional&
 *      operator= (const optional&);
 *    
 *      // Pointer-like interface.
 *      //
 *    public:
 *      const X*
 *      operator-> () const;
 *    
 *      X*
 *      operator-> ();
 *    
 *      const X&
 *      operator* () const;
 *    
 *      X&
 *      operator* ();
 *    
 *      typedef void (optional::*bool_convertible) ();
 *      operator bool_convertible () const;
 *    
 *      // Get/set interface.
 *      //
 *    public:
 *      // Check if an element is present or not
 *      bool
 *      present () const;
 *    
 *      // Get an element if it is present - an error otherwise
 *      const X&
 *      get () const;
 *    
 *      // Get an element if it is present - an error otherwise
 *      X&
 *      get ();
 *    
 *      // Makes a deep copy.
 *      //
 *      void
 *      set (const X&);
 *    
 *      // Assumes ownership.
 *      //
 *      void
 *      set (std::[auto|unique]_ptr<X>);
 *    
 *      // Detach and return the contained value.
 *      //
 *      std::[auto|unique]_ptr<X>
 *      detach ();
 *    
 *      // Delete an element
 *      void
 *      reset ();
 *    };
 *    
 *    template <typename X>
 *    bool
 *    operator== (const optional<X>&, const optional<X>&);
 *    
 *    template <typename X>
 *    bool
 *    operator!= (const optional<X>&, const optional<X>&);
 *    
 *    template <typename X>
 *    bool
 *    operator< (const optional<X>&, const optional<X>&);
 *    
 *    template <typename X>
 *    bool
 *    operator> (const optional<X>&, const optional<X>&);
 *    
 *    template <typename X>
 *    bool
 *    operator<= (const optional<X>&, const optional<X>&);
 *    
 *    template <typename X>
 *    bool
 *    operator>= (const optional<X>&, const optional<X>&);
 *   
 * ~~~~
 * 
 * \section sequenceinfo _sequence and _base type interface
 * 
 * See section 2.8.3 of the Code Synthesis C++/Tree Mapping User Manual.
 * 
 * Types named "_sequence" or derived from an "_base" class provide an interface 
 * of a standard C++ sequence (e.g., std::vector). Familiar functions like
 * push_back() etc. may be used.
 *
 * \section optionalexample Example
 *
 * The type onem2m::AE contains an optional element of type onem2m::labels. The onem2m::labels type is a sequence type 
 * derived from onem2m::labels_base.
 * 
 * The following code shows how the labels parameter in the AE may be populated
 * 
 * ~~~~
 *    auto ae = AE();
 *    auto lb = labels ();
 *    lb.push_back("Type/updateSensor");
 *    ae.labels(lb);
 * ~~~~
 */


#ifndef ONEM2M_HXX_
#define ONEM2M_HXX_



#include <string>
#include <vector>
#include <functional>

#include "cdtHeads.hxx"

/* Notes on adding new resource types
 * 
 * To add a new resource type to the library consider the following steps:
 * - Include all needed XSD in cdt/xsd_vx_y_z_ed. Note that this may include selecting which part(s) of the XSD files are needed
 * - If needed, add a new root element to the end of the CDT-commonTypes file
 * - Update "cdt/build_command" to add any new CDT files to the list to be processed
 * - Update the data structures at the start of "cdt/cxxhelper/cxxhelper.py"
 * - Run the "build_command" to build CodeSynthesis output and other automatically generated files
 * - Add any enumerated values needed from CDT-enumeration types to this file
 */ 

 /**
  * @brief Public interface for functions and classes in the OS-IoT library.
  *
  * Functions are used to perform oneM2M operations. Classes represent oneM2M resources and operations.
  */
namespace onem2m
{
  // Magic values defined in CDT-enumerationTypes.xsd

 /** 
  * Identifiers for types of resource or operations in prototol
  */
  enum onem2mResourceType: ::xml_schema::integer {
    resourceTypeUnknown = 0, ///< Unknown resource type or no resource type included
    resourceTypeAccessControlPolicy,
    resourceTypeAE,
    resourceTypeContainer,
    resourceTypeContentInstance,
    resourceTypeCSEBase,
    resourceTypePollingChannel = 15,
    resourceTypeSubscription = 23,
    resourceTypeFlexContainerResource = 28,
    operationTypeNotification = 30500, ///< Value used to designate a notification operation, which is not a resource
    objectTypeListOfURIs = 30501 ///< Valuse used to designate a listOfURIs object, which is not a resource
  };
 
  /**
   * Identifiers for accessControlOperations
   */
  enum onem2mAccesControlOperation: ::xml_schema::integer {
    accessControlCreate = 1, ///< Create access control
    accessControlRetrieve = 2, ///< Retrieve access control
    accessControlUpdate = 4, ///< Update access control
    accessControlDelete = 8, ///< Delete access control
    accessControlNotify = 16, ///<Notify access control
    accessControlDiscover = 32, ///<Discover access control
    accessControlAll = 63 ///<All access control
  };

  
  /**
  * Identifiers for operations
  */
  enum onem2mOperation: ::xml_schema::integer {
    operationCreate = 1, ///< Create a resource
    operationRetrieve,  ///< Retrieve a resource
    operationUpdate,    ///< Update a resource
    operationDelete,    ///< Delete a resource
    operationNotify     ///< Notify of a resource change

  };


  /**
  * Identifiers for notificationContentType
  */
  enum onem2mMotificationContentType: ::xml_schema::integer {
    nctAllAttributes = 1, ///< All atributes
    nctModifiedAttributes,  ///< Modified attributes
    nctResourceId,    ///< Resource Id
  };


  /** 
  * Identifers for notification event types
  */
  enum onem2mNotificationEventType: ::xml_schema::integer {
    updateOfResource = 1,
    deleteOfResource,
    createOfDirectChildResource,
    deleteOfDirectChildResource,
    retrieveOfContainerResourceWithNoChildResource
  };


  /** 
  * Identifiers for protocols to used sending commands
  */
  enum onem2mProtocol {
    protocolXml = 1, ///< XML protocol
    protocolJson ///< JSON protocol
  };

  /**
  * Identifiers for transports used sending comments
  */
  enum onem2mTransport {
     transportHttp = 1, ///< HTTP transport
     transportHttpsCertificate, ///< HTTPS transport with certificate-based security
     transportHttpsPsk ///< HTTPS transport with PSK security
  };

  /** 
  * Identifiers for responseStatusCodes
  *
  */
  enum onem2mResponseStatusCode: long {
    rcACCEPTED = 1000,
    rcOK = 2000,
    rcCREATED = 2001,
    rcDELETED = 2002,
    rcUPDATED = 2004,
    rcBAD_REQUEST = 4000,
    rcNOT_FOUND = 4004,
    rcOPERATION_NOT_ALLOWED = 4005,
    rcREQUEST_TIMEOUT = 4008,
    rcSUBSCRIPTION_CREATOR_HAS_NO_PRIVILEGE = 4101,
    rcCONTENTS_UNACCEPTABLE = 4102,
    rcORIGINATOR_HAS_NO_PRIVILEGE = 4103,
    rcGROUP_REQUEST_IDENTIFIER_EXISTS = 4104,
    rcCONFLICT = 4105,
    rcORIGINATOR_HAS_NOT_REGISTERED = 4106,
    rcSECURITY_ASSOCIATION_REQUIRED = 4107,
    rcINVALID_CHILD_RESOURCE_TYPE = 4108,
    rcNO_MEMBERS = 4109,
    rcGROUP_MEMBER_TYPE_INCONSISTENT = 4110,
    rcESPRIM_UNSUPPORTED_OPTION = 4111,
    rcESPRIM_UNKNOWN_KEY_ID = 4112,
    rcGESPRIM_UNKNOWN_ORIG_RAND_ID = 4113,
    rcESPRIM_UNKNOWN_RECV_RAND_ID = 4114,
    rcGESPRIM_BAD_MAC = 4115,
    rcESPRIM_IMPERSONATION_ERROR = 4116,
    rcINTERNAL_SERVER_ERROR = 5000,
    rcNOT_IMPLEMENTED = 5001,
    rcTARGET_NOT_REACHABLE = 5103,
    rcRECEIVER_HAS_NO_PRIVILEGE = 5105,
    rcALREADY_EXISTS = 5106,
    rcTARGET_NOT_SUBSCRIBABLE = 5203,
    rcSUBSCRIPTION_VERIFICATION_INITIATION_FAILED = 5204,
    rcSUBSCRIPTION_HOST_HAS_NO_PRIVILEGE = 5205,
    rcNON_BLOCKING_REQUEST_NOT_SUPPORTED = 5206,
    rcNOT_ACCEPTABLE = 5207,
    rcDISCOVERY_DENIED_BY_IPE = 5208,
    rcGROUP_MEMBERS_NOT_RESPONDED = 5209,
    rcESPRIM_DECRYPTION_ERROR = 5210,
    rcESPRIM_ENCRYPTION_ERROR1 = 5211,
    rcESPRIM_ENCRYPTION_ERROR2 = 5212,
    rcSPARQL_UPDATE_ERROR = 6003,
    rcEXTERNAL_OBJECT_NOT_FOUND = 6005,
    rcMAX_NUMBER_OF_MEMBER_EXCEEDED = 6010,
    rcMGMT_SESSION_CANNOT_BE_ESTABLISHED = 6020,
    rcMGMT_SESSION_ESTABLISHMENT_TIMEOUT = 6021,
    rcINVALID_CMDTYPE = 6022,
    rcINVALID_ARGUMENTS = 6023,
    rcINSUFFICIENT_ARGUMENTS = 6024,
    rcMGMT_CONVERSION_ERROR = 6025,
    rcMGMT_CANCELLATION_FAILED = 6026,
    rcALREADY_COMPLETE = 6028,
    rcMGMT_COMMAND_NOT_CANCELLABLE = 6029
  };

  /**
  * Identifiers for results and errors
  *
  * HTTP responses take their normal values. Errors reported by libCURL are returned as 1000+ their native Curl values.
  */
  enum onem2mResult: long {
    onem2mHttpCONTINUE = 100,
    onem2mHttpSWITCHING_PROTOCOLS = 101,
    onem2mHttpOK = 200,
    onem2mHttpCREATED = 201,
    onem2mHttpACCEPTED = 202,
    onem2mHttpNON_AUTHORITATIVE_INFORMATION = 203,
    onem2mHttpNO_CONTENT = 204,
    onem2mHttpRESET_CONTENT = 205,
    onem2mHttpPARTIAL_CONTENT = 206,
    onem2mHttpMULTIPLE_CHOICES = 300,
    onem2mHttpMOVED_PERMANENTLY = 301,
    onem2mHttpFOUND = 302,
    onem2mHttpSEE_OTHER = 303,
    onem2mHttpNOT_MODIFIED = 304,
    onem2mHttpUSE_PROXY = 305,
    onem2mHttpTEMPORARY_REDIRECT = 307,
    onem2mHttpBAD_REQUEST = 400,
    onem2mHttpUNAUTHORIZED = 401,
    onem2mHttpPAYMENT_REQUIRED = 402,
    onem2mHttpFORBIDDEN = 403,
    onem2mHttpNOT_FOUND = 404,
    onem2mHttpMETHOD_NOT_ALLOWED = 405,
    onem2mHttpNOT_ACCEPTABLE = 406,
    onem2mHttpPROXY_AUTHENTICATION_REQUIRED = 407,
    onem2mHttpREQUEST_TIMEOUT = 408,
    onem2mHttpCONFLICT = 409,
    onem2mHttpGONE = 410,
    onem2mHttpLENGTH_REQUIRED = 411,
    onem2mHttpPRECONDITION_FAILED = 412,
    onem2mHttpREQUEST_ENTITY_TOO_LARGE = 413,
    onem2mHttpREQUEST_URI_TOO_LONG = 414,
    onem2mHttpUNSUPPORTED_MEDIA_TYPE = 415,
    onem2mHttpREQUESTED_RANGE_NOT_SATISFIABLE = 416,
    onem2mHttpEXPECTATION_FAILED = 417,
    onem2mHttpUNPROCESSABLE_ENTITY = 422,
    onem2mHttpTOO_MANY_REQUESTS = 429,
    onem2mHttpINTERNAL_SERVER_ERROR = 500,
    onem2mHttpNOT_IMPLEMENTED = 501,
    onem2mHttpBAD_GATEWAY = 502,
    onem2mHttpSERVICE_UNAVAILABLE = 503,
    onem2mHttpGATEWAY_TIMEOUT = 504,
    onem2mHttpHTTP_VERSION_NOT_SUPPORTED = 505,
    onem2mCURLE_UNSUPPORTED_PROTOCOL = 1001,    
    onem2mCURLE_FAILED_INIT,             
    onem2mCURLE_URL_MALFORMAT,           
    onem2mCURLE_NOT_BUILT_IN,
    onem2mCURLE_COULDNT_RESOLVE_PROXY,   
    onem2mCURLE_COULDNT_RESOLVE_HOST,    
    onem2mCURLE_COULDNT_CONNECT,         
    onem2mCURLE_WEIRD_SERVER_REPLY,      
    onem2mCURLE_REMOTE_ACCESS_DENIED,
    onem2mCURLE_FTP_ACCEPT_FAILED, 
    onem2mCURLE_FTP_WEIRD_PASS_REPLY,    
    onem2mCURLE_FTP_ACCEPT_TIMEOUT,
    onem2mCURLE_FTP_WEIRD_PASV_REPLY,    
    onem2mCURLE_FTP_WEIRD_227_FORMAT,    
    onem2mCURLE_FTP_CANT_GET_HOST,       
    onem2mCURLE_HTTP2,
    onem2mCURLE_FTP_COULDNT_SET_TYPE,    
    onem2mCURLE_PARTIAL_FILE,            
    onem2mCURLE_FTP_COULDNT_RETR_FILE,   
    onem2mCURLE_OBSOLETE20,              
    onem2mCURLE_QUOTE_ERROR,             
    onem2mCURLE_HTTP_RETURNED_ERROR,     
    onem2mCURLE_WRITE_ERROR,             
    onem2mCURLE_OBSOLETE24,              
    onem2mCURLE_UPLOAD_FAILED,           
    onem2mCURLE_READ_ERROR,              
    onem2mCURLE_OUT_OF_MEMORY,           
    onem2mCURLE_OPERATION_TIMEDOUT,      
    onem2mCURLE_OBSOLETE29,              
    onem2mCURLE_FTP_PORT_FAILED,         
    onem2mCURLE_FTP_COULDNT_USE_REST,    
    onem2mCURLE_OBSOLETE32,              
    onem2mCURLE_RANGE_ERROR,             
    onem2mCURLE_HTTP_POST_ERROR,         
    onem2mCURLE_SSL_CONNECT_ERROR,       
    onem2mCURLE_BAD_DOWNLOAD_RESUME,     
    onem2mCURLE_FILE_COULDNT_READ_FILE,  
    onem2mCURLE_LDAP_CANNOT_BIND,        
    onem2mCURLE_LDAP_SEARCH_FAILED,      
    onem2mCURLE_OBSOLETE40,              
    onem2mCURLE_FUNCTION_NOT_FOUND,      
    onem2mCURLE_ABORTED_BY_CALLBACK,     
    onem2mCURLE_BAD_FUNCTION_ARGUMENT,   
    onem2mCURLE_OBSOLETE44,              
    onem2mCURLE_INTERFACE_FAILED,        
    onem2mCURLE_OBSOLETE46,              
    onem2mCURLE_TOO_MANY_REDIRECTS,      
    onem2mCURLE_UNKNOWN_OPTION,          
    onem2mCURLE_TELNET_OPTION_SYNTAX,    
    onem2mCURLE_OBSOLETE50,              
    onem2mCURLE_PEER_FAILED_VERIFICATION,
    onem2mCURLE_GOT_NOTHING,             
    onem2mCURLE_SSL_ENGINE_NOTFOUND,     
    onem2mCURLE_SSL_ENGINE_SETFAILED, 
    onem2mCURLE_SEND_ERROR,              
    onem2mCURLE_RECV_ERROR,              
    onem2mCURLE_OBSOLETE57,              
    onem2mCURLE_SSL_CERTPROBLEM,         
    onem2mCURLE_SSL_CIPHER,              
    onem2mCURLE_SSL_CACERT,              
    onem2mCURLE_BAD_CONTENT_ENCODING,    
    onem2mCURLE_LDAP_INVALID_URL,        
    onem2mCURLE_FILESIZE_EXCEEDED,       
    onem2mCURLE_USE_SSL_FAILED,          
    onem2mCURLE_SEND_FAIL_REWIND,
    onem2mCURLE_SSL_ENGINE_INITFAILED,   
    onem2mCURLE_LOGIN_DENIED,
    onem2mCURLE_TFTP_NOTFOUND,           
    onem2mCURLE_TFTP_PERM,               
    onem2mCURLE_REMOTE_DISK_FULL,        
    onem2mCURLE_TFTP_ILLEGAL,            
    onem2mCURLE_TFTP_UNKNOWNID,          
    onem2mCURLE_REMOTE_FILE_EXISTS,      
    onem2mCURLE_TFTP_NOSUCHUSER,         
    onem2mCURLE_CONV_FAILED,             
    onem2mCURLE_CONV_REQD,
    onem2mCURLE_SSL_CACERT_BADFILE,
    onem2mCURLE_REMOTE_FILE_NOT_FOUND,   
    onem2mCURLE_SSH,
    onem2mCURLE_SSL_SHUTDOWN_FAILED,
    onem2mCURLE_AGAIN,
    onem2mCURLE_SSL_CRL_BADFILE, 
    onem2mCURLE_SSL_ISSUER_ERROR,
    onem2mCURLE_FTP_PRET_FAILED,         
    onem2mCURLE_RTSP_CSEQ_ERROR,         
    onem2mCURLE_RTSP_SESSION_ERROR,      
    onem2mCURLE_FTP_BAD_FILE_LIST,       
    onem2mCURLE_CHUNK_FAILED,            
    onem2mCURLE_NO_CONNECTION_AVAILABLE,
    onem2mCURLE_SSL_PINNEDPUBKEYNOTMATCH,
    onem2mCURLE_SSL_INVALIDCERTSTATUS,   
    onem2mCURLE_HTTP2_STREAM,
    onem2mErrCantOpenServer = 2001,
    onem2mErrServerAlreadyRunning
  };

  /**
   * Function template to be used if the application specifies a callback function to receive notifications.
   */
  typedef std::function<onem2mResponseStatusCode(std::string, std::string&, ::onem2m::notification*)> notificationCallbackFn;

  extern bool _debug;

  std::string _getHostName();

  void initialize ();

  void terminate ();

  void setProtocol( const onem2mProtocol );

  onem2mProtocol getProtocol();  

  void setTransport( const onem2mTransport );

  onem2mTransport getTransport();

  void setCipherList( const std::string ); 

  std::string getCipherList();

  void setVerifyPeer( const bool );

  bool getVerifyPeer();

  void setVerifyHost( const bool );

  bool getVerifyHost();

  void setSubjectAltNameAllowed( const std::string );

  std::string getSubjectAltNameAllowed();

  void setCaInfo( const std::string );

  std::string getCaInfo();
  
  void setCaPath( const std::string );

  std::string getCaPath();

  void setSslCert( const std::string );

  std::string getSslCert();

  void setSslKey( const std::string );

  std::string getSslKey();

  void setKeyPasswd( const std::string );

  std::string getKeyPasswd();

  void setPskIdentity( const std::string );

  std::string getPskIdentity();

  void setPskKey( const std::string );

  std::string getPskKey();
 
  void setHostName( const std::string name );

  void setFrom( const std::string newFrom );

  void setMaxAcceptSize( const size_t newMaxAcceptSize );
  
  size_t getMaxAcceptSize();

  std::string getFrom();

  void setDebug( bool debugOut );

  std::unique_ptr< ::xml_schema::type > retrieveResource(const std::string& address, const std::string& requestId,
                                                         long& result,
                                                         ::xml_schema::integer& resObjType);    
 
  std::unique_ptr< ::xml_schema::type > deleteResource(const std::string& address, const std::string& requestId,
                                                       long& result,
                                                       ::xml_schema::integer& resObjType);  
    
  std::unique_ptr< ::xml_schema::type > createResource(const std::string& address, const std::string& requestId,
                                                       ::xml_schema::type& resource,
                                                       long& result,
                                                       ::xml_schema::integer& resObjType);

  std::unique_ptr< ::xml_schema::type > updateResource(const std::string& address, const std::string& requestId,
                                                       const ::xml_schema::type* resource,
                                                       long& result,
                                                       ::xml_schema::integer& resObjType);  

  std::unique_ptr< ::xml_schema::type > updateResource(const std::string& address, const std::string& requestId,
                                                       const ::xml_schema::type& resource,
                                                       long& result,
                                                       ::xml_schema::integer& resObjType);         
  
  std::unique_ptr< ::xml_schema::type > _send(const std::string& address, const std::string& requestId,
                   const ::xml_schema::type* resource, const ::xml_schema::integer* resourceType,
                   const onem2mOperation operation,
                   long& result, ::xml_schema::integer& resObjType);

  bool getHttpServerRunning();

  long startHttpServer( const std::vector<std::string>& addressRegex, long port, notificationCallbackFn callback );

  void stopHttpServer( );

}




#endif
