/* 
 *	SPDX-License-Identifier:	BSD-3-Clause 
 * The above license addresses copyright obligations. Please refer to Section 10.4 of the 
 * ATIS Operating Procedures for information about patent licensing obligations under 
 * ATIS' Intellectual Property Rights Policy.
 */


/** \file CDT-notification-custom.hxx
 * This file extends the type notificationBase which is automatically generated 
 * from the XSD to add methods to access the "representation" object.
 */

namespace onem2m
{

  /**
   * @brief Class corresponding to the notification schema type. 
   *
   * Note that
   * the special method getRepresentationObject to get the
   * the object that can represent the resource that caused the notification.
   *
   * Most of the methods are contained in the notificationBase parent class.
   *
   * @nosubgrouping
   */

  class notification: public notificationBase {
  public:

    /**
     * @brief Create an instance from the ultimate base and
     * initializers for required elements and attributes.
     */
    notification ();

    /**
     * @brief Create an instance from a DOM element.
     *
     * @param e A DOM element to extract the data from.
     * @param f Flags to create the new instance with.
     * @param c A pointer to the object that will contain the new
     * instance.
     */
    notification (const ::xercesc::DOMElement& e,
                  ::xml_schema::flags f = 0,
                  ::xml_schema::container* c = 0);

    /**
     * @brief Copy constructor.
     *
     * @param x An instance to make a copy of.
     * @param f Flags to create the copy with.
     * @param c A pointer to the object that will contain the copy.
     *
     * For polymorphic object models use the @c _clone function instead.
     */
    notification (const notification& x,
                  ::xml_schema::flags f = 0,
                  ::xml_schema::container* c = 0);

    /**
     * @brief Copy the instance polymorphically.
     *
     * @param f Flags to create the copy with.
     * @param c A pointer to the object that will contain the copy.
     * @return A pointer to the dynamically allocated copy.
     *
     * This function ensures that the dynamic type of the instance is
     * used for copying and should be used for polymorphic object
     * models instead of the copy constructor.
     */
    virtual notification*
    _clone (::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0) const;

   
    /**
     * @brief Set the contents of the "representation object". For internal use in libary only.
     *
     * @param rot enum of the object type.
     * @param rop A std::unique_ptr to the object. Ownership of this pointer is transferred to the notification object.
     */
    void _setRepresentationObject( ::xml_schema::integer rot, std::unique_ptr <::xml_schema::type> rop );

    /**
     * @brief Get the object in the "representation" parameter of the notification (if any).
     *
     * @param rot Variable which will be updated to show the type of the object.
     * @return A pointer to the object.
     *
     * If the function returns with rot==0 (resourceTypeUnknown) the value of the pointer is undefined.
     * Else, rot will indicate the onem2mResourceType and the method will return a pointer to an
     * object of the corresponding type. Ownership of the object is retained by the parent notification
     * object and will automatically be destroyed when the parent notification object is deleted.
     */
    ::xml_schema::type * getRepresentationObject( ::xml_schema::integer & rot) const;

    
    private:

    ::xml_schema::integer representationObjectType = 0;
    std::unique_ptr <::xml_schema::type> representationObjectPtr;
    
  };
}
