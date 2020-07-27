# BackEndSmsEdit
Tests with C++ Webserver using Drogon and Postgresql

Drogon : https://github.com/an-tao/drogon

Database used;

```SQL
CREATE SCHEMA sms;

CREATE SEQUENCE sms.seq_user;
CREATE SEQUENCE sms.seq_vendor;

CREATE TABLE sms."user" (
    id_user bigint DEFAULT nextval('sms.seq_user'::regclass) NOT NULL,
    ds_user_name character varying(50) NOT NULL,
    ds_user_password character varying(50) NOT NULL,
    fk_vendor bigint NOT NULL
);

CREATE TABLE sms.vendor (
    id_vendor bigint DEFAULT nextval('sms.seq_vendor'::regclass) NOT NULL,
    ds_vendor_name character varying(150) NOT NULL,
    ds_vendor_user_token character varying(150) NOT NULL
    ds_vendor_token character varying(150) NOT NULL
);
```
