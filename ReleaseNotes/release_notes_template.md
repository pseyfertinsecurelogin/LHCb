{% set used = [] -%}
{% macro section(labels) -%}
{% for mr in select_mrs(merge_requests, labels, used) %}
- {{mr.title}}, !{{mr.iid}} (@{{mr.author.username}}) {{find_tasks(mr)}}  
  {{mr.description|mdindent(2)}}
{% endfor %}
{%- endmacro %}

{{date}} {{project}} {{version}}
===

This version uses Gaudi v30r3 and LCG_93 with ROOT 6.12.06.
<p>
This version is released on `master` branch.

Built relative to LHCb v50rX, with the following changes:

### New features
{{ section(['new feature']) }}

### Enhancements
{{ section(['enhancement']) }}

### Bug fixes
{{ section(['bug fix']) }}

### Code modernisations and cleanups
{{ section(['cleanup', 'modernisation']) }}

### Monitoring changes
{{ section(['monitoring']) }}

### Changes to tests
{{ section(['testing']) }}

### Other
{{ section([[]]) }}