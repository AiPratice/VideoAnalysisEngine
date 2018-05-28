--
-- PostgreSQL database dump
--

-- Dumped from database version 10.3 (Ubuntu 10.3-1)
-- Dumped by pg_dump version 10.3 (Ubuntu 10.3-1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: frame_faces; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.frame_faces (
    frame_face_id integer NOT NULL,
    frame_id bigint,
    video_id integer,
    person_id integer,
    top integer,
    "left" integer,
    width integer,
    height integer,
    created_at timestamp without time zone
);


ALTER TABLE public.frame_faces OWNER TO postgres;

--
-- Name: frame_faces_frame_face_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.frame_faces_frame_face_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.frame_faces_frame_face_id_seq OWNER TO postgres;

--
-- Name: frame_faces_frame_face_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.frame_faces_frame_face_id_seq OWNED BY public.frame_faces.frame_face_id;


--
-- Name: frame_objects; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.frame_objects (
    frame_object_id integer NOT NULL,
    frame_id bigint NOT NULL,
    object_class_id integer NOT NULL,
    video_id integer NOT NULL,
    created_at timestamp without time zone,
    prob double precision,
    "left" integer,
    top integer,
    width integer,
    height integer
);


ALTER TABLE public.frame_objects OWNER TO postgres;

--
-- Name: frames; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.frames (
    frame_id bigint NOT NULL,
    video_id integer NOT NULL,
    img_path text,
    feature_path text,
    second integer,
    is_shot_frame boolean
);


ALTER TABLE public.frames OWNER TO postgres;

--
-- Name: frames_frame_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.frames_frame_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.frames_frame_id_seq OWNER TO postgres;

--
-- Name: frames_frame_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.frames_frame_id_seq OWNED BY public.frames.frame_id;


--
-- Name: frames_video_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.frames_video_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.frames_video_id_seq OWNER TO postgres;

--
-- Name: frames_video_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.frames_video_id_seq OWNED BY public.frames.video_id;


--
-- Name: object_classes; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.object_classes (
    object_class_id integer NOT NULL,
    cn_name text,
    en_name text,
    created_at timestamp without time zone,
    updated_at timestamp without time zone
);


ALTER TABLE public.object_classes OWNER TO postgres;

--
-- Name: TABLE object_classes; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE public.object_classes IS '物体类别';


--
-- Name: person_faces; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.person_faces (
    person_face_id integer NOT NULL,
    path text,
    created_at timestamp without time zone,
    person_id integer
);


ALTER TABLE public.person_faces OWNER TO postgres;

--
-- Name: TABLE person_faces; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE public.person_faces IS '用于训练人脸检索';


--
-- Name: persons; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.persons (
    person_id integer NOT NULL,
    name text,
    description text,
    created_at timestamp without time zone,
    updated_at timestamp without time zone
);


ALTER TABLE public.persons OWNER TO postgres;

--
-- Name: persons_person_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.persons_person_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.persons_person_id_seq OWNER TO postgres;

--
-- Name: persons_person_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.persons_person_id_seq OWNED BY public.persons.person_id;


--
-- Name: train_faces_train_face_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.train_faces_train_face_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.train_faces_train_face_id_seq OWNER TO postgres;

--
-- Name: train_faces_train_face_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.train_faces_train_face_id_seq OWNED BY public.person_faces.person_face_id;


--
-- Name: video_objects_frame_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.video_objects_frame_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.video_objects_frame_id_seq OWNER TO postgres;

--
-- Name: video_objects_frame_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.video_objects_frame_id_seq OWNED BY public.frame_objects.frame_id;


--
-- Name: video_objects_frame_object_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.video_objects_frame_object_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.video_objects_frame_object_id_seq OWNER TO postgres;

--
-- Name: video_objects_frame_object_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.video_objects_frame_object_id_seq OWNED BY public.frame_objects.frame_object_id;


--
-- Name: video_objects_video_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.video_objects_video_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.video_objects_video_id_seq OWNER TO postgres;

--
-- Name: video_objects_video_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.video_objects_video_id_seq OWNED BY public.frame_objects.video_id;


--
-- Name: videos; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.videos (
    video_id integer NOT NULL,
    name text,
    description text,
    path text,
    gdindex_path text,
    status smallint,
    created_at timestamp without time zone,
    updated_at timestamp without time zone,
    object_status smallint,
    face_status smallint
);


ALTER TABLE public.videos OWNER TO postgres;

--
-- Name: COLUMN videos.object_status; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public.videos.object_status IS '物体检测状态(0:未检测  1:正在检测  2:检测完毕)';


--
-- Name: videos_video_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.videos_video_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.videos_video_id_seq OWNER TO postgres;

--
-- Name: videos_video_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.videos_video_id_seq OWNED BY public.videos.video_id;


--
-- Name: frame_faces frame_face_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.frame_faces ALTER COLUMN frame_face_id SET DEFAULT nextval('public.frame_faces_frame_face_id_seq'::regclass);


--
-- Name: frame_objects frame_object_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.frame_objects ALTER COLUMN frame_object_id SET DEFAULT nextval('public.video_objects_frame_object_id_seq'::regclass);


--
-- Name: frame_objects frame_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.frame_objects ALTER COLUMN frame_id SET DEFAULT nextval('public.video_objects_frame_id_seq'::regclass);


--
-- Name: frame_objects video_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.frame_objects ALTER COLUMN video_id SET DEFAULT nextval('public.video_objects_video_id_seq'::regclass);


--
-- Name: frames frame_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.frames ALTER COLUMN frame_id SET DEFAULT nextval('public.frames_frame_id_seq'::regclass);


--
-- Name: frames video_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.frames ALTER COLUMN video_id SET DEFAULT nextval('public.frames_video_id_seq'::regclass);


--
-- Name: person_faces person_face_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.person_faces ALTER COLUMN person_face_id SET DEFAULT nextval('public.train_faces_train_face_id_seq'::regclass);


--
-- Name: persons person_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.persons ALTER COLUMN person_id SET DEFAULT nextval('public.persons_person_id_seq'::regclass);


--
-- Name: videos video_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.videos ALTER COLUMN video_id SET DEFAULT nextval('public.videos_video_id_seq'::regclass);


--
-- Name: frame_faces frame_faces_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.frame_faces
    ADD CONSTRAINT frame_faces_pkey PRIMARY KEY (frame_face_id);


--
-- Name: frames frames_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.frames
    ADD CONSTRAINT frames_pkey PRIMARY KEY (frame_id);


--
-- Name: object_classes object_classes_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.object_classes
    ADD CONSTRAINT object_classes_pkey PRIMARY KEY (object_class_id);


--
-- Name: persons persons_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.persons
    ADD CONSTRAINT persons_pkey PRIMARY KEY (person_id);


--
-- Name: person_faces train_faces_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.person_faces
    ADD CONSTRAINT train_faces_pkey PRIMARY KEY (person_face_id);


--
-- Name: frame_objects video_objects_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.frame_objects
    ADD CONSTRAINT video_objects_pkey PRIMARY KEY (frame_object_id);


--
-- Name: videos videos_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.videos
    ADD CONSTRAINT videos_pkey PRIMARY KEY (video_id);


--
-- Name: fki_person_face_fk; Type: INDEX; Schema: public; Owner: postgres
--

CREATE INDEX fki_person_face_fk ON public.person_faces USING btree (person_id);


--
-- Name: frame_objects fk_video_object_frame; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.frame_objects
    ADD CONSTRAINT fk_video_object_frame FOREIGN KEY (frame_id) REFERENCES public.frames(frame_id);


--
-- Name: frame_objects fk_video_object_object; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.frame_objects
    ADD CONSTRAINT fk_video_object_object FOREIGN KEY (object_class_id) REFERENCES public.object_classes(object_class_id);


--
-- Name: frame_objects fk_video_object_video; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.frame_objects
    ADD CONSTRAINT fk_video_object_video FOREIGN KEY (video_id) REFERENCES public.videos(video_id);


--
-- Name: frames frame_video_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.frames
    ADD CONSTRAINT frame_video_fk FOREIGN KEY (video_id) REFERENCES public.videos(video_id);


--
-- Name: person_faces person_face_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.person_faces
    ADD CONSTRAINT person_face_fk FOREIGN KEY (person_id) REFERENCES public.persons(person_id);


--
-- PostgreSQL database dump complete
--

