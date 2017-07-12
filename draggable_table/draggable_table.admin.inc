<?php

/**
 * @file
 * Draggable Table Drupal Form editing UI.
 */

/**
 * Implements hook_form().
 */
function draggable_table_settings_form($form, &$form_state) {
  $months_var = variable_get('draggable_table_store', array());
  $numeric_month = array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);

  $rows = array(
    // Make it a tree for easier traversing of the entered values on submission.
    '#tree' => TRUE,
    // First row.
    'default' => array(
      'numeric_month' => array(
        '#type' => 'select',
        '#options' => $numeric_month,
        '#default_value' => '',
      ),
      'month' => array(
        '#type' => 'textfield',
        '#default_value' => '',
      ),
      'weight' => array(
        '#type' => 'textfield',
        '#default_value' => '',
        '#attributes' => array(
          'class' => array('dt-order-weight'),
        ),
      ),
    ),
  );

  if (count($months_var) >= 12) {
    unset($rows['default']);
  }

  $form['months'] = array(
    // Theme this part of the form as a table.
    '#theme' => 'draggable_table',
    // Pass header information to the theme function.
    '#header' => array(t('Numeric Month'), t('Month'), t('Weight')),
    'rows' => $rows,
  );

  foreach ($months_var as $id => $row) {
    $rows += array(
      $id => array(
        'numeric_month' => array(
          '#type' => 'select',
          '#options' => $numeric_month,
          '#default_value' => $row['nmonth'],
        ),
        'month' => array(
          '#type' => 'textfield',
          '#default_value' => $row['month'],
        ),
        'weight' => array(
          '#type' => 'textfield',
          '#default_value' => $row['weight'],
          '#attributes' => array(
            'class' => array('dt-order-weight'),
          ),
        ),
      ),
    );
  }

  // Rows in the form table.
  $form['months']['rows'] = $rows;

  $form['numeric'] = array(
    '#type' => 'fieldset',
    '#title' => t('Age'),
  );
  $form['numeric']['age'] = array(
    '#type' => 'textfield',
    '#title' => t('Age'),
    '#description' => t('How old are you?'),
    '#default_value' => variable_get('draggable_table_age', 0),
    '#required' => TRUE,
  );

  $form = system_settings_form($form);
  // $form['#validate'] = array('draggable_table_validate');.
  $form['#submit'] = array('draggable_table_submit');

  return $form;
}

/**
 * Implements hook_form_submit().
 */
function draggable_table_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  variable_set('draggable_table_age', $form_values['age']);

  $store = array();
  foreach ($form_values['rows'] as $rid => $row) {
    if (!empty($row['month'])) {
      $store[] = array(
        'nmonth' => $row['numeric_month'],
        'month' => $row['month'],
        'weight' => $row['weight'],
      );
    }
  }

  if (!empty($store)) {
    usort($store, 'draggable_table_arraysort');
  }

  variable_set('draggable_table_store', $store);

  drupal_set_message(t('Saved!'));
}
