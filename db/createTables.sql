--
-- PostgreSQL database dump
--

-- Dumped from database version 10.5
-- Dumped by pg_dump version 11.0 (Debian 11.0-1.pgdg90+2)

SET client_encoding = 'UTF8';
--
-- Name: weathersource; Type: TABLE; Schema: foundation
--

CREATE TABLE foundation.weathersource (
    id integer NOT NULL,
    locationname character varying NOT NULL,
    active boolean DEFAULT false NOT NULL
);

--
-- Name: weather; Type: TABLE; Schema: world
--

CREATE TABLE world.weather (
    id integer NOT NULL,
    locationname character varying NOT NULL,
    temperature double precision NOT NULL,
    humidity integer NOT NULL,
    windspeed double precision NOT NULL,
    surveydate timestamp without time zone DEFAULT now() NOT NULL,
    winddeg integer NOT NULL,
    visibility integer NOT NULL,
    dt integer DEFAULT 0 NOT NULL
);


ALTER TABLE ONLY foundation.weathersource
    ADD CONSTRAINT foundation_id PRIMARY KEY (id);

--
-- Name: weather weather_pkey; Type: CONSTRAINT; Schema: world
--

ALTER TABLE ONLY world.weather
    ADD CONSTRAINT weather_pkey PRIMARY KEY (id, surveydate);