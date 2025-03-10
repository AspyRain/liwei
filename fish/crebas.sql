/*==============================================================*/
/* DBMS name:      MySQL 5.0                                    */
/* Created on:     2025/3/9 14:24:50                            */
/*==============================================================*/


drop table if exists Admin;

drop table if exists fishRecords;

drop table if exists wqRecords;

/*==============================================================*/
/* Table: Admin                                                 */
/*==============================================================*/
create table Admin
(
   userName             varchar(20) not null,
   password             varchar(20) not null,
   primary key (userName)
);

/*==============================================================*/
/* Table: fishRecords                                           */
/*==============================================================*/
create table fishRecords
(
   id                   int not null,
   speed                decimal(5,2) not null,
   size                 decimal(5,2) not null,
   status               smallint not null,
   date                 date not null,
   primary key (id)
);

/*==============================================================*/
/* Table: wqRecords                                             */
/*==============================================================*/
create table wqRecords
(
   水质记录id               int not null,
   phValue              decimal(3,2) not null,
   turbidity            decimal(6,2) not null,
   temperature          decimal(4,2) not null,
   date                 date not null,
   status               smallint not null,
   primary key (水质记录id)
);

