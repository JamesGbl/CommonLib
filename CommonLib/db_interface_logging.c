#include "db_interface_logging.h" 
#include "llogging.h"
#include "lmemory.h"

DbConnection_class *DbConnection_logging_class();
lbool DbConnection_logging_sql_exec( DbConnection *self, const char *sql, lerror **error );
DbPrepared *DbConnection_logging_sql_prepare( DbConnection *self, const char *sql, lerror **error );
void DbConnection_logging_destroy(DbConnection *self);
const char *DbConnection_logging_get_type(DbConnection *self);
DbIterator* DbConnection_logging_sql_retrieve( DbConnection *self, const char *sql, lerror **error );

typedef struct DbConnection_Logging {
	DbConnection parent;
    DbConnection *forwarder;
} DbConnection_Logging;

DbConnection *DbConnection_create_logging(DbConnection *parent) {
	DbConnection_Logging *self = NULL;

	self = (DbConnection_Logging *)lmalloc( sizeof(DbConnection_Logging) );
	DbConnection_init( (DbConnection *)self, DbConnection_logging_class() );
    self->forwarder = parent;

	return (DbConnection *)self;
}

DbConnection_class *DbConnection_logging_class() {
	static DbConnection_class oClass;

	oClass.destroy = DbConnection_logging_destroy;
	oClass.sql_exec = DbConnection_logging_sql_exec;
	oClass.sql_prepare = DbConnection_logging_sql_prepare;
	oClass.sql_retrieve = DbConnection_logging_sql_retrieve;
    oClass.get_type = DbConnection_logging_get_type;

	return &oClass;
}

void DbConnection_logging_destroy(DbConnection *self) {
	DbConnection_Logging *logging = (DbConnection_Logging *)self;
    if (self==NULL) return;
}

lbool DbConnection_logging_sql_exec( DbConnection *self, const char *sql, lerror **error ) {
	DbConnection_Logging *logging = (DbConnection_Logging *)self;

    l_assert(self!=NULL);
    l_assert(sql!=NULL);
    l_assert(error==NULL || *error==NULL);

    l_info("[%s] sql_exec: %s", DbConnection_get_type(self), sql);

    return DbConnection_sql_exec(logging->forwarder, sql, error);
}

DbPrepared *DbConnection_logging_sql_prepare( DbConnection *self, const char *sql, lerror **error ) {
    l_assert(self!=NULL);
    l_assert(sql!=NULL);
    l_assert(error==NULL || *error==NULL);

    l_info("[%s] sql_prepare: %s", DbConnection_get_type(self), sql);
    
    return DbConnection_sql_prepare(self, sql, error);
}

const char *DbConnection_logging_get_type(DbConnection *self) {
    DbConnection_Logging *logging = (DbConnection_Logging *)self;
    l_assert(self!=NULL);
    return DbConnection_get_type(logging->forwarder);
}    

DbIterator* DbConnection_logging_sql_retrieve( DbConnection *self, const char *sql, lerror **error ) {
    DbConnection_Logging *logging = (DbConnection_Logging *)self;

    l_assert(self!=NULL);
    l_assert(sql!=NULL);
    l_assert(error==NULL || *error==NULL);

    l_info("[%s] sql_retrieve: %s", DbConnection_get_type(self), sql);
    return DbConnection_sql_retrieve(logging->forwarder, sql, error);
}
