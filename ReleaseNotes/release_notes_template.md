{% set categories = [
    'selection', 'hlt1', 'hlt2', 'Configuration',
    'Decoding', 'Tracking', 'PV finding', 'Muon', 'Calo', 'RICH', 'Jets',
    'PID', 'Composites', 'Filters', 'Functors',
    'Event model', 'Persistency',
    'MC checking', 'Monitoring', 'Luminosity',
    'Core', 'Conditions', 'Utilities',
    'Simulation',  'Tuples', 'Accelerators',
    'Flavour tagging',
    'Build',
    ] -%}
{% set used_mrs = [] -%}
{% macro section(labels, used=used_mrs) -%}
{% for mr in order_by_label(select_mrs(merge_requests, labels, used), categories) -%}
  {% set mr_labels = categories|select("in", mr.labels)|list -%}
- {% if mr_labels %}{{mr_labels|map('label_ref')|join(' ')}} | {% endif -%}
  {{mr.title|sentence}}, {{mr.reference}} (@{{mr.author.username}}){% if mr.issue_refs %} [{{mr.issue_refs|join(',')}}]{% endif %}{% if 'highlight' in mr.labels %} :star:{% endif %}
{# {{mr.description|mdindent(2)}} -#}
{% endfor -%}
{% endmacro -%}
{{date}} {{project}} {{version}}
===

This version uses Phys v31rX, Rec v31rX, Lbcom v31rX, LHCb v51rX, Gaudi v33r0 and LCG_96b with ROOT 6.18.04.

This version is released on `master` branch.
Built relative to Moore v51rX, with the following changes:

### New features ~"new feature"

{{ section(['new feature']) }}

### Fixes ~"bug fix" ~workaround

{{ section(['bug fix', 'workaround']) }}

### Enhancements ~enhancement

{{ section(['enhancement']) }}

### Code cleanups and changes to tests ~modernisation ~cleanup ~testing

{{ section(['cleanup', 'modernisation', 'testing']) }}

### Documentation ~Documentation

{# Collect documentation independently, may lead to repeated entries -#}
{{ section(['Documentation'], used=None) }}
{# Mark as used such documentation does not appear under Other -#}
{% set dummy = section(['Documentation']) -%}

### Other

{{ section([[]]) }}
