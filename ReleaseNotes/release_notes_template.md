{% set used = [] -%}
{% macro section(labels) -%}
{% for mr in select_mrs(merge_requests, labels, used) %}
- {{mr.title}}, !{{mr.iid}} (@{{mr.author.username}}) {{find_tasks(mr)}}  
  {{mr.description|mdindent(2)}}
{% endfor %}
{%- endmacro %}

{{date}} {{project}} {{version}}
===

This version uses Gaudi v32r2 and LCG_96b with ROOT 6.18.04.
<p>
This version is released on `master` branch.

Built relative to LHCb v50rX, with the following changes:

### New features
{{ section(['new feature']) }}

### Enhancements
{{ section(['enhancement']) }}

### Thread safety
{{ section(['thread safety']) }}

### Bug fixes
{{ section(['bug fix']) }}

### Code modernisations and cleanups
{{ section(['cleanup', 'modernisation']) }}

### Monitoring changes
{{ section(['monitoring']) }}

### Changes to tests
{{ section(['testing']) }}

### Documentation
{{ section(['documentation']) }}

### Other
{{ section([[]]) }}
