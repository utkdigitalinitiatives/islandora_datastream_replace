# Islandora Datastream Replace

## Introduction

This module provides a Drush script that can be used to bulk replace existing datastreams
given a Solr query.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Troubleshooting/Issues

## Usage
Output of ```drush islandora_datastream_replace --help:```

```
Scans a source directory and replaces datastreams according to the given namespace and the filenames.

Examples:
 drush -u 1 islandora_datastream_replace  --source=/tmp --namespace=islandora

Options:
 --dsid                                    The datastream id of to be exported datastream. Required.
 --source                                  The directory to get the files of the datastreams from. Required.
  ```


The user option (-u) needs to be specified or errors could be
encountered when attempting to write the contents of the datastream to a file.

## Maintainers/Sponsors

Current maintainers:

* [pc37utn](https://github.com/pc37utn)

This project has been sponsored by:


## Development

If you would like to contribute to this module, please check out our helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the Islandora.ca site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
