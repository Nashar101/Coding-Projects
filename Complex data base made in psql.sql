/*
* File: Assignment2_SubmissionTemplate.sql
* 
* 1) Rename this file according to the instructions in the assignment statement.
* 2) Use this file to insert your solution.
*
*
* Author: <Bempah>, <Gilead>
* Student ID Number: <2296232>
* Institutional mail prefix: <gxb035>
*/


/*
*  Assume a user account 'fsad' with password 'fsad2022' with permission
* to create  databases already exists. You do NO need to include the commands
* to create the user nor to give it permission in you solution.
* For your testing, the following command may be used:
*
* CREATE USER fsad PASSWORD 'fsad2022' CREATEDB;
* GRANT pg_read_server_files TO fsad;
*/


/* *********************************************************
* Exercise 1. Create the Smoked Trout database
* 
************************************************************ */

-- The first time you login to execute this file with \i it may
-- be convenient to change the working directory.
\cd 'C:/Users/nasha/Documents/SqlAssignment'
  -- In PostgreSQL, folders are identified with '/'
\connect postgres;
DROP DATABASE IF EXISTS "SmokedTrout";

-- 1) Create a database called SmokedTrout.


-- 2) Connect to the database

CREATE DATABASE "SmokedTrout"
	WITH
	OWNER = fsad
	ENCODING = 'UTF8'
	CONNECTION LIMIT = -1;

\connect "SmokedTrout" fsad;


/* *********************************************************
* Exercise 2. Implement the given design in the Smoked Trout database
* 
************************************************************ */

-- 1) Create a new ENUM type called materialState for storing the raw material state

CREATE TYPE "materialState" AS ENUM('Solid', 'Liquid', 'Gas', 'Plasma');

-- 2) Create a new ENUM type called materialComposition for storing whether
-- a material is Fundamental or Composite.

CREATE TYPE "materialComposition" AS ENUM('Fundamental', 'Composite');

-- 3) Create the table TradingRoute with the corresponding attributes.

CREATE TABLE public."TradingRoute"(
	"MonitoringKey" integer,
	"FleetSize" integer,
	"OperatingCompany" text,
	"LastYearRevenue" real,
	PRIMARY KEY ("MonitoringKey")
);

-- 4) Create the table Planet with the corresponding attributes.

CREATE TABLE public."Planet"(
	"PlanetID" integer,
	"StarSystem" text,
	"Name" text,
	"Population" integer,
	PRIMARY KEY ("PlanetID")
);

-- 5) Create the table SpaceStation with the corresponding attributes.

CREATE TABLE public."SpaceStation"(
	"StationID" integer,
	"PlanetID" integer, 
	"Name" text,
	"Longitude" text,
	"Latitude" text,
	Primary Key ("StationID")
);

ALTER TABLE "SpaceStation"
	ADD FOREIGN KEY ("PlanetID")
	REFERENCES public."Planet" ("PlanetID") MATCH SIMPLE
	ON UPDATE CASCADE
	ON DELETE CASCADE
	NOT VALID;

-- 6) Create the parent table Product with the corresponding attributes.

CREATE TABLE public."Product"(
	"ProductID" integer,
	"Name" text,	
	"VolumePerTon" decimal,
	"ValuePerTon" decimal,
	Primary Key ("ProductID")
);


-- 7) Create the child table RawMaterial with the corresponding attributes.

CREATE TABLE public."RawMaterial"(
	"FundamentalOrComposite" "materialComposition",
	"State" "materialState"
) 
	INHERITS(public."Product");


-- 8) Create the child table ManufacturedGood.

CREATE TABLE public."ManufacturedGood"(
) 
	INHERITS(public."Product"); 

-- 9) Create the table MadeOf with the corresponding attributes.

CREATE TABLE public."MadeOf"(
	"ManufacturedGoodID" integer,
	"ProductID" integer
);
	

-- 10) Create the table Batch with the corresponding attributes.

CREATE TABLE public."Batch"(
	"BatchID" integer,
	"ProductID" integer,
	"ExtractionOrManufacturingDate" date,
	"OriginalFrom" integer,
	PRIMARY KEY ("BatchID")
);

ALTER TABLE IF EXISTS public."Batch"
	ADD FOREIGN KEY ("OriginalFrom")
	REFERENCES public."Planet" ("PlanetID") MATCH SIMPLE
	ON UPDATE CASCADE
	ON DELETE CASCADE
	NOT VALID;

-- 11) Create the table Sells with the corresponding attributes.

CREATE TABLE public."Sells"(
	"BatchID" integer,
	"StationID" integer
);

ALTER TABLE IF EXISTS public."Sells"
	ADD FOREIGN KEY ("BatchID")
	REFERENCES public."Batch" ("BatchID") MATCH SIMPLE
	ON UPDATE CASCADE
	ON DELETE CASCADE
	NOT VALID;

ALTER TABLE IF EXISTS public."Sells"
	ADD FOREIGN KEY ("StationID")
	REFERENCES public."SpaceStation" ("StationID") MATCH SIMPLE
	ON UPDATE CASCADE
	ON DELETE CASCADE
	NOT VALID;

-- 12)  Create the table Buys with the corresponding attributes.

CREATE TABLE public."Buys"(
	"BatchID" integer,
	"StationID" integer
);

ALTER TABLE IF EXISTS public."Buys"
	ADD FOREIGN KEY ("BatchID")
	REFERENCES public."Batch" ("BatchID") MATCH SIMPLE
	ON UPDATE CASCADE
	ON DELETE CASCADE
	NOT VALID;

ALTER TABLE IF EXISTS public."Buys"
	ADD FOREIGN KEY ("StationID")
	REFERENCES public."SpaceStation" ("StationID") MATCH SIMPLE
	ON UPDATE CASCADE
	ON DELETE CASCADE
	NOT VALID;

-- 13)  Create the table CallsAt with the corresponding attributes.

CREATE TABLE "CallsAt"(
	"MonitoringKey" integer,
	"StationID" integer,
	"VisitOrder" integer
);

ALTER TABLE IF EXISTS "CallsAt"
	ADD FOREIGN KEY ("StationID")
	REFERENCES public."SpaceStation" ("StationID") MATCH SIMPLE
	ON UPDATE CASCADE
	ON DELETE CASCADE
	NOT VALID;


ALTER TABLE IF EXISTS "CallsAt"
	ADD FOREIGN KEY ("MonitoringKey")
	REFERENCES public."TradingRoute" ("MonitoringKey") MATCH SIMPLE
	ON UPDATE CASCADE
	ON DELETE CASCADE
	NOT VALID;

-- 14)  Create the table Distance with the corresponding attributes.
CREATE TABLE "Distance"(
	"PlanetOrigin" integer,
	"PlanetDestination" integer,
	"AvgDistance" real
);

ALTER TABLE IF EXISTS "Distance"
	ADD FOREIGN KEY ("PlanetOrigin")
	REFERENCES public."Planet" ("PlanetID") MATCH SIMPLE
	ON UPDATE CASCADE
	ON DELETE CASCADE
	NOT VALID;
ALTER TABLE IF EXISTS "Distance"
	ADD FOREIGN KEY ("PlanetOrigin")
	REFERENCES public."Planet" ("PlanetID") MATCH SIMPLE
	ON UPDATE CASCADE
	ON DELETE CASCADE
	NOT VALID;

/* *********************************************************
* Exercise 3. Populate the Smoked Trout database
* 
************************************************************ */
/* *********************************************************
* NOTE: The copy statement is NOT standard SQL.
* The copy statement does NOT permit on-the-fly renaming columns,
* hence, whenever necessary, we:
* 1) Create a dummy table with the column name as in the file
* 2) Copy from the file to the dummy table
* 3) Copy from the dummy table to the real table
* 4) Drop the dummy table (This is done further below, as I keep
*    the dummy table also to imporrt the other columns)
************************************************************ */



-- 1) Unzip all the data files in a subfolder called data from where you have your code file 
-- NO CODE GOES HERE. THIS STEP IS JUST LEFT HERE TO KEEP CONSISTENCY WITH THE ASSIGNMENT STATEMENT

-- 2) Populate the table TradingRoute with the data in the file TradeRoutes.csv.
 CREATE TABLE "Dummy" (
 	"MonitoringKey" SERIAL ,
 	"FleetSize" int ,
 	"OperatingCompany" varchar (40),
 	"LastYearRevenue" real NOT NULL);

\copy "Dummy" FROM ./data/TradeRoutes.csv WITH (FORMAT CSV , HEADER );

INSERT INTO "TradingRoute" ("MonitoringKey", "FleetSize", "OperatingCompany", "LastYearRevenue")
SELECT "MonitoringKey", "FleetSize", "OperatingCompany", "LastYearRevenue" FROM "Dummy";

DROP TABLE "Dummy"; 

-- 3) Populate the table Planet with the data in the file Planets.csv.

CREATE TABLE "Dummy" (
 	"PlanetID" SERIAL ,
 	"StarSystem" text ,
 	"Planet" varchar (40),
 	"Population_inMillions_" int NOT NULL);

\copy "Dummy" FROM ./data/Planets.csv WITH (FORMAT CSV , HEADER );

INSERT INTO "Planet" ("PlanetID", "StarSystem", "Name", "Population")
SELECT "PlanetID", "StarSystem", "Planet", "Population_inMillions_" FROM "Dummy";

DROP TABLE "Dummy"; 

-- 4) Populate the table SpaceStation with the data in the file SpaceStations.csv.

CREATE TABLE "Dummy" (
 	"StationID" SERIAL ,
 	"PlanetID" int ,
 	"SpaceStations" text,
	"Longitude" text,
	"Latitude" text
 	);

\copy "Dummy" FROM ./data/SpaceStations.csv WITH (FORMAT CSV , HEADER );

INSERT INTO "SpaceStation" ("StationID", "PlanetID", "Name", "Longitude", "Latitude")
SELECT "StationID", "PlanetID", "SpaceStations", "Longitude", "Latitude" FROM "Dummy";

DROP TABLE "Dummy"; 

-- 5) Populate the tables RawMaterial and Product with the data in the file Products_Raw.csv.

CREATE TABLE "Dummy" (
 	"ProductID" SERIAL ,
 	"Product" text ,
 	"Composite" text,
	"VolumePerTon" real,
	"ValuePerTon" real,
	"State" "materialState"
 	);

\copy "Dummy" FROM ./data/Products_Raw.csv WITH (FORMAT CSV , HEADER );

UPDATE "Dummy" 
SET "Composite" = REPLACE ("Composite", 'No', 'Fundamental');

UPDATE "Dummy" 
SET "Composite" = REPLACE ("Composite", 'Yes', 'Composite');

ALTER TABLE "Dummy"
ALTER COLUMN "Composite" TYPE "materialComposition"
USING "Composite"::text::"materialComposition",
Alter COLUMN "Composite" SET DEFAULT 'Fundamental';

INSERT INTO "RawMaterial" ("ProductID", "Name", "FundamentalOrComposite", "VolumePerTon", "ValuePerTon", "State")
SELECT "ProductID", "Product", "Composite", "VolumePerTon", "ValuePerTon", "State" FROM "Dummy";

DROP TABLE "Dummy";

-- 6) Populate the tables ManufacturedGood and Product with the data in the file  Products_Manufactured.csv.

CREATE TABLE "Dummy" (
 	"ProductID" SERIAL ,
 	"Product" text ,
	"VolumePerTon" real,
	"ValuePerTon" real
 	);

\copy "Dummy" FROM ./data/Products_Manufactured.csv WITH (FORMAT CSV , HEADER );

INSERT INTO "ManufacturedGood" ("ProductID", "Name", "VolumePerTon", "ValuePerTon")
SELECT "ProductID", "Product", "VolumePerTon", "ValuePerTon" FROM "Dummy";

DROP TABLE "Dummy";

-- 7) Populate the table MadeOf with the data in the file MadeOf.csv.

\copy "MadeOf" FROM ./data/MadeOf.csv WITH (FORMAT CSV , HEADER );

-- 8) Populate the table Batch with the data in the file Batches.csv.

CREATE TABLE "Dummy"(
	"BatchID" integer,
	"ProductID" integer,
	"ExtractionOrManufacturingDate" date,
	"OriginalFrom" integer
);

\copy "Dummy" FROM ./data/Batches.csv WITH (FORMAT CSV , HEADER );

INSERT INTO "Batch" ("BatchID", "ProductID", "ExtractionOrManufacturingDate", "OriginalFrom")
SELECT "BatchID", "ProductID", "ExtractionOrManufacturingDate", "OriginalFrom" FROM "Dummy";

DROP TABLE "Dummy";
-- 9) Populate the table Sells with the data in the file Sells.csv.

\copy "Sells" FROM ./data/Sells.csv WITH (FORMAT CSV , HEADER );

-- 10) Populate the table Buys with the data in the file Buys.csv.

\copy "Buys" FROM ./data/Buys.csv WITH (FORMAT CSV , HEADER );

-- 11) Populate the table CallsAt with the data in the file CallsAt.csv.

\copy "CallsAt" FROM ./data/CallsAt.csv WITH (FORMAT CSV , HEADER );

-- 12) Populate the table Distance with the data in the file PlanetDistances.csv.

CREATE TABLE "Dummy"(
	"PlanetOrigin" integer,
	"PlanetDestination" integer,
	"Distance" real
);

\copy "Dummy" FROM ./data/PlanetDistances.csv WITH (FORMAT CSV , HEADER );

INSERT INTO "Distance" ("PlanetOrigin", "PlanetDestination", "AvgDistance")
SELECT "PlanetOrigin", "PlanetDestination", "Distance" FROM "Dummy";

DROP TABLE "Dummy";

/* *********************************************************
* Exercise 4. Query the database
* 
************************************************************ */

-- 4.1 Report last year taxes per company

-- 1) Add an attribute Taxes to table TradingRoute

ALTER TABLE "TradingRoute"
ADD COLUMN "Taxes" real;

-- 2) Set the derived attribute taxes as 12% of LastYearRevenue

UPDATE public."TradingRoute"
SET "Taxes" = "LastYearRevenue" * 0.12;

-- 3) Report the operating company and the sum of its taxes group by company.

SELECT "OperatingCompany", SUM ("Taxes") FROM "TradingRoute" GROUP BY "OperatingCompany";

-- 4.2 What's the longest trading route in parsecs?

-- 1) Create a dummy table RouteLength to store the trading route and their lengths.

CREATE TABLE "RouteLength"(
	"RouteMonitoringKey" integer,
	"RouteTotalDistance" decimal,
	PRIMARY KEY("RouteMonitoringKey")
);
-- 2) Create a view EnrichedCallsAt that brings together trading route, space stations and planets.

CREATE VIEW "EnrichedCallsAt" AS SELECT "TradingRoute"."MonitoringKey", "TradingRoute"."OperatingCompany",
	"SpaceStation"."StationID","VisitOrder", "SpaceStation"."Name" ,
	"Planet"."PlanetID", "StarSystem","Planet"."Name" AS "Sname" FROM "TradingRoute" 
	INNER JOIN "CallsAt" 
	ON "TradingRoute"."MonitoringKey" = "CallsAt"."MonitoringKey"
	INNER JOIN "SpaceStation"
	ON "CallsAt"."StationID" = "SpaceStation"."StationID"
	INNER JOIN "Planet"
	ON "SpaceStation"."PlanetID" = "Planet"."PlanetID";

-- 3) Add the support to execute an anonymous code block as follows;

DO
$$
DECLARE 
"routeDistance" real := 0.0; 
"hopDistance" real := 0.0;
"rRoute" record; 
"rHop" record; 
query text;
BEGIN
FOR "rRoute" IN SELECT "MonitoringKey" FROM "TradingRoute"
LOOP
query := 'CREATE OR REPLACE VIEW "PortsOfCall" AS'
 ||  ' SELECT "PlanetID" , "VisitOrder"' 
 ||  ' FROM "EnrichedCallsAt"' 
 ||  ' WHERE "MonitoringKey" ='  || "rRoute"."MonitoringKey"
 ||  ' ORDER BY "VisitOrder"' ;
EXECUTE query ;
query := ' CREATE OR REPLACE VIEW "Hops" AS
 	SELECT p2."PlanetID" AS "PlanetOrigin", p1."PlanetID" AS "Planet Destination"
 	FROM "PortsOfCall" p1 INNER JOIN "PortsOfCall" p2 ON p1."VisitOrder" = p2."VisitOrder" + 1';
EXECUTE query ;
"routeDistance" := 0.0;
FOR "rHop" IN SELECT "PlanetOrigin", "Planet Destination" FROM "Hops"
LOOP
query := 'CREATE OR REPLACE VIEW "PartialDistance" AS 
	 SELECT "AvgDistance" FROM "Distance"
	 WHERE "PlanetOrigin" =' || "rHop"."PlanetOrigin" || 'AND 
	 "PlanetDestination" =' || "rHop"."Planet Destination";
EXECUTE query ;
SELECT "AvgDistance" INTO "hopDistance" FROM "PartialDistance";
"routeDistance" := "routeDistance" + "hopDistance";
END LOOP;
INSERT INTO "RouteLength" ("RouteMonitoringKey", "RouteTotalDistance")
 	VALUES ("rRoute"."MonitoringKey", "routeDistance");
END LOOP;
END;
$$;

-- 4) Within the declare section, declare a variable of type real to store a route total distance.

-- 5) Within the declare section, declare a variable of type real to store a hop partial distance.

-- 6) Within the declare section, declare a variable of type record to iterate over routes.

-- 7) Within the declare section, declare a variable of type record to iterate over hops.

-- 8) Within the declare section, declare a variable of type text to transiently build dynamic queries.

-- 9) Within the main body section, loop over routes in TradingRoutes

-- 10) Within the loop over routes, get all visited planets (in order) by this trading route.

-- 11) Within the loop over routes, execute the dynamic view

-- 12) Within the loop over routes, create a view Hops for storing the hops of that route. 

-- 13) Within the loop over routes, initialize the route total distance to 0.0.

-- 14) Within the loop over routes, create an inner loop over the hops

-- 15) Within the loop over hops, get the partial distances of the hop. 

-- 16)  Within the loop over hops, execute the dynamic view and store the outcome INTO the hop partial distance.

-- 17)  Within the loop over hops, accumulate the hop partial distance to the route total distance.

-- 18)  Go back to the routes loop and insert into the dummy table RouteLength the pair (RouteMonitoringKey,RouteTotalDistance).

-- 19)  Within the loop over routes, drop the view for Hops (and cascade to delete dependent objects).

DROP VIEW "Hops" CASCADE;

-- 20)  Within the loop over routes, drop the view for PortsOfCall (and cascade to delete dependent objects).

DROP VIEW "PortsOfCall" CASCADE;

-- 21)  Finally, just report the longest route in the dummy table RouteLength.
SELECT * FROM "RouteLength" WHERE "RouteTotalDistance" = (SELECT MAX("RouteTotalDistance") FROM "RouteLength");
