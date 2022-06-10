#include "tree_sitter.h"

extern VALUE mTreeSitter;

VALUE cLanguage;

TSLanguage *value_to_language(VALUE self) {
  TSLanguage *language;

  Data_Get_Struct(self, TSLanguage, language);

  return language;
}

VALUE new_language(const TSLanguage *language) {
  return Data_Wrap_Struct(cLanguage, NULL, NULL, (void *)language);
}

static VALUE language_symbol_count(VALUE self) {
  TSLanguage *language = value_to_language(self);

  return INT2NUM(ts_language_symbol_count(language));
}

static VALUE language_symbol_name(VALUE self, VALUE symbol) {
  TSLanguage *language = value_to_language(self);

  return rb_str_new_cstr(ts_language_symbol_name(language, NUM2INT(symbol)));
}

static VALUE language_symbol_for_name(VALUE self, VALUE string,
                                      VALUE is_named) {
  TSLanguage *language = value_to_language(self);
  char *str = StringValuePtr(string);
  uint32_t length = (uint32_t)RSTRING_LEN(string);
  bool named = RTEST(is_named);

  return INT2NUM(ts_language_symbol_for_name(language, str, length, named));
}

static VALUE language_field_count(VALUE self) {
  TSLanguage *language = value_to_language(self);

  return INT2NUM(ts_language_field_count(language));
}

static VALUE language_field_name_for_id(VALUE self, VALUE field_id) {
  TSLanguage *language = value_to_language(self);

  return rb_str_new_cstr(
      ts_language_field_name_for_id(language, NUM2INT(field_id)));
}

static VALUE language_field_id_for_name(VALUE self, VALUE name) {
  TSLanguage *language = value_to_language(self);
  const char *str = StringValuePtr(name);
  uint32_t length = (uint32_t)RSTRING_LEN(name);

  return INT2NUM(ts_language_field_id_for_name(language, str, length));
}

static VALUE language_symbol_type(VALUE self, VALUE symbol) {
  TSLanguage *language = value_to_language(self);

  return new_symbol_type(ts_language_symbol_type(language, NUM2INT(symbol)));
}

void init_language(void) {
  cLanguage = rb_define_class_under(mTreeSitter, "Language", rb_cObject);

  /* Class methods */
  rb_define_method(cLanguage, "symbol_count", language_symbol_count, 0);
  rb_define_method(cLanguage, "symbol_name", language_symbol_name, 1);
  rb_define_method(cLanguage, "symbol_for_name", language_symbol_for_name, 2);
  rb_define_method(cLanguage, "field_count", language_field_count, 0);
  rb_define_method(cLanguage, "field_name_for_id", language_field_name_for_id,
                   1);
  rb_define_method(cLanguage, "field_id_for_name", language_field_id_for_name,
                   1);
  rb_define_method(cLanguage, "symbol_type", language_symbol_type, 1);
}
