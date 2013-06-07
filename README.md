Drupal 7 DrUUID Module
=======================


Overview
--------

This module provides a field for adding universally unique identifiers (UUID) to Drupal entities, called a DRUUID. It also 
lets you choose which type of valid RFC 4211 compliant UUID type you would like use, i.e. v3, v4 or v5.


Features
--------

 1. Provides a UUID Field 
    - Creates a field that can be attached to drupal entity bundles, and generates a UUID of various types.
 
 2. Provides different UUID Types
    - Lets you choose with RFC compliant UUID code to use when generating DRUUIDS.


Usage
-----

In your custom code => 

$my_uuid = druuid_create($version = '4', $namespace = NULL);

* Named-based UUID.
* $v3uuid = DrUUID::v3('1546058f-5a25-4334-85ae-e68f2a44bbaf', 'SomeRandomString');
* $v5uuid = DrUUID::v5('1546058f-5a25-4334-85ae-e68f2a44bbaf', 'SomeRandomString');

* Pseudo-random UUID
* $v4uuid = DrUUID::v4();

The module now also provides an admin screen, in order to apply the field instance to the various entity 
bundle types within the system.


Requirements
------------

Drupal 7.x only


Installation
------------

Place this module folder in the your drupal module directory and then 
navigate to administer >> modules (admin/modules). 

Enable Druuid.


KNOWN PROBLEMS
------------

None at this time...

CREDITS
------------

@morlenefisher
@dvdtoth
@modernfidelity


SIMILAR PROJECTS
------------

UUID : http://drupal.org/project/uuid
