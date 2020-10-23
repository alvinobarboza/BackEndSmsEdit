# BackEndSmsEdit
Tests with C++ Webserver using Drogon and Postgresql

Drogon : https://github.com/an-tao/drogon

Database used;

```SQL
CREATE SCHEMA sms;

CREATE SEQUENCE sms.seq_user;
CREATE SEQUENCE sms.seq_vendor;
CREATE SEQUENCE sms.seq_contact;
CREATE SEQUENCE sms.seq_customer;

CREATE TABLE sms.vendor (
    id_vendor bigint DEFAULT nextval('sms.seq_vendor'::regclass) NOT NULL,
    ds_vendor_name character varying(150) NOT NULL,
    ds_vendor_user_token character varying(150) NOT NULL,
    ds_vendor_token character varying(150) NOT NULL,
    ds_vendor_url character varying(150) NOT NULL,
    PRIMARY KEY (id_vendor)
);

CREATE TABLE sms."user" (
    id_user bigint DEFAULT nextval('sms.seq_user'::regclass) NOT NULL,
    ds_user_name character varying(50) NOT NULL,
    ds_user_password character varying(50) NOT NULL,
    ds_user_profile integer,
    fk_vendor BIGINT NOT NULL REFERENCES sms.vendor (id_vendor),
    ds_profile_name character varying(50) NOT NULL,
    PRIMARY KEY (id_user)
);

CREATE TABLE sms.contact (
    id_contact bigint DEFAULT nextval('sms.seq_contact'::regclass) NOT NULL,
    ds_contact_email character varying(150) NOT NULL,
    ds_contact_phone1 character varying(150) NOT NULL,
	ds_contact_phone2 character varying(150) NOT NULL,    
    PRIMARY KEY(id_contact)
);

CREATE TABLE sms.customer (
    id_customer bigint DEFAULT nextval('sms.seq_customer'::regclass) NOT NULL,
    ds_customer_name character varying(50) NOT NULL,
    ds_customer_lastname character varying(50) NOT NULL,
    ds_customer_login character varying(50) NOT NULL,
    ds_customer_profile_name character varying(50) NOT NULL,
	ds_customer_sms_pair_id character varying(10),
    dt_customer_birthdate date NOT NULL,
	fk_contact BIGINT NOT NULL REFERENCES sms.contact(id_contact),
    PRIMARY KEY (id_customer)    
);
```
