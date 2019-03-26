<?php


/**
 * Menu callback for admin/structure/block.
 *
 * @param $theme
 *   The theme to display the administration page for. If not provided, defaults
 *   to the currently used theme.
 */
function test_brief_news($theme = NULL) {
  global $theme_key;

  drupal_theme_initialize();

  if (!isset($theme)) {
    // If theme is not specifically set, rehash for the current theme.
    $theme = $theme_key;
  }

  // Fetch and sort blocks.
  $blocks = block_admin_display_prepare_blocks1($theme);

  return drupal_get_form('block_admin_display_form1', $blocks, $theme);
}

/**
 * Prepares a list of blocks for display on the blocks administration page.
 *
 * @param $theme
 *   The machine-readable name of the theme whose blocks should be returned.
 *
 * @return
 *   An array of blocks, as returned by _block_rehash(), sorted by region in
 *   preparation for display on the blocks administration page.
 *
 * @see block_admin_display_form()
 */
function block_admin_display_prepare_blocks1($theme) {
  $blocks = _block_rehash($theme);
  $compare_theme = &drupal_static('_block_compare:theme');
  $compare_theme = $theme;
  usort($blocks, '_block_compare');
  return $blocks;
}

/**
 * Form constructor for the main block administration form.
 *
 * @param $blocks
 *   An array of blocks, as returned by block_admin_display_prepare_blocks().
 * @param $theme
 *   A string representing the name of the theme to edit blocks for.
 * @param $block_regions
 *   (optional) An array of regions in which the blocks will be allowed to be
 *   placed. Defaults to all visible regions for the theme whose blocks are
 *   being configured. In all cases, a dummy region for disabled blocks will
 *   also be displayed.
 *
 * @return
 *   An array representing the form definition.
 *
 * @ingroup forms
 * @see block_admin_display_form_submit()
 */
function block_admin_display_form1($form, &$form_state, $blocks, $theme, $block_regions = NULL) {

  $form['#attached']['css'] = array(drupal_get_path('module', 'block') . '/block.css');

  // Get a list of block regions if one was not provided.
  if (!isset($block_regions)) {
    $block_regions = system_region_list($theme, REGIONS_VISIBLE);
  }

  // Weights range from -delta to +delta, so delta should be at least half
  // of the amount of blocks present. This makes sure all blocks in the same
  // region get an unique weight.
  $weight_delta = round(count($blocks) / 2);

  // Build the form tree.
  $form['edited_theme'] = array(
    '#type' => 'value',
    '#value' => $theme,
  );
  $form['block_regions'] = array(
    '#type' => 'value',
    // Add a last region for disabled blocks.
    '#value' => $block_regions + array(BLOCK_REGION_NONE => BLOCK_REGION_NONE),
  );
  $form['blocks'] = array();
  $form['#tree'] = TRUE;

  foreach ($blocks as $i => $block) {
    $key = $block['module'] . '_' . $block['delta'];
    $form['blocks'][$key]['module'] = array(
      '#type' => 'value',
      '#value' => $block['module'],
    );
    $form['blocks'][$key]['delta'] = array(
      '#type' => 'value',
      '#value' => $block['delta'],
    );
    $form['blocks'][$key]['info'] = array(
      '#markup' => check_plain($block['info']),
    );
    $form['blocks'][$key]['theme'] = array(
      '#type' => 'hidden',
      '#value' => $theme,
    );
    $form['blocks'][$key]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $block['weight'],
      '#delta' => $weight_delta,
      '#title_display' => 'invisible',
      '#title' => t('Weight for @block block', array('@block' => $block['info'])),
    );
    $form['blocks'][$key]['region'] = array(
      '#type' => 'select',
      '#default_value' => $block['region'] != BLOCK_REGION_NONE ? $block['region'] : NULL,
      '#empty_value' => BLOCK_REGION_NONE,
      '#title_display' => 'invisible',
      '#title' => t('Region for @block block', array('@block' => $block['info'])),
      '#options' => $block_regions,
    );
    $form['blocks'][$key]['configure'] = array(
      '#type' => 'link',
      '#title' => t('configure'),
      '#href' => 'admin/structure/block/manage/' . $block['module'] . '/' . $block['delta'] . '/configure',
    );
    if ($block['module'] == 'block') {
      $form['blocks'][$key]['delete'] = array(
        '#type' => 'link',
        '#title' => t('delete'),
        '#href' => 'admin/structure/block/manage/' . $block['module'] . '/' . $block['delta'] . '/delete',
     );
    }
  }
  // Do not allow disabling the main system content block when it is present.
  if (isset($form['blocks']['system_main']['region'])) {
    $form['blocks']['system_main']['region']['#required'] = TRUE;
  }

  $form['actions'] = array(
    '#tree' => FALSE,
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save blocks'),
  );

  return $form;
}

/**
 * Form submission handler for block_admin_display_form().
 *
 * @see block_admin_display_form()
 */
function block_admin_display_form1_submit($form, &$form_state) {
  $transaction = db_transaction();
  try {
    foreach ($form_state['values']['blocks'] as $block) {
      $block['status'] = (int) ($block['region'] != BLOCK_REGION_NONE);
      $block['region'] = $block['status'] ? $block['region'] : '';
      db_update('block')
        ->fields(array(
          'status' => $block['status'],
          'weight' => $block['weight'],
          'region' => $block['region'],
        ))
        ->condition('module', $block['module'])
        ->condition('delta', $block['delta'])
        ->condition('theme', $block['theme'])
        ->execute();
    }
  }
  catch (Exception $e) {
    $transaction->rollback();
    watchdog_exception('block', $e);
    throw $e;
  }
  drupal_set_message(t('The block settings have been updated.'));
  cache_clear_all();
}
