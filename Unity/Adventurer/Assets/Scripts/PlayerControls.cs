using NaughtyAttributes;

using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerControls : MonoBehaviour
{
    [Header("Character Settings")]
    [SerializeField] private float walkSpeed = 4f;
    [SerializeField] private float sprintSpeed = 10f;

    [Header("Debugging")]
    [SerializeField, ReadOnly] private Vector2 movement;
    [SerializeField, ReadOnly] private bool isSprinting;

    [Header("Input Actions")]
    [SerializeField] private InputActionReference movementActionRef;
    [SerializeField] private InputActionReference sprintActionRef;
    [SerializeField] private InputActionReference meleeActionRef;

    private float moveSpeed;
    private float rotAngle;
    
    private Vector3 inputVector;
    
    private new Rigidbody rigidbody;
    private Animator animator;
    
    private static readonly int stabbing = Animator.StringToHash("Stabbing");
    private static readonly int vertical = Animator.StringToHash("Vertical");

    private void Awake()
    {
        meleeActionRef.action.started += Melee;
    }

    private void Start()
    {
        animator = GetComponent<Animator>();
        rigidbody = GetComponent<Rigidbody>();

        moveSpeed = walkSpeed;
    }

    private void Update() => GetInputs();

    private void FixedUpdate() => Movement();

    private void Movement()
    {
        moveSpeed = isSprinting ? sprintSpeed : walkSpeed;
        
        rigidbody.MovePosition(transform.position + inputVector * (moveSpeed * Time.fixedDeltaTime));
        
        if(inputVector.magnitude >= 0.1f)
        {
            animator.SetFloat(vertical, isSprinting ? 2 : 1);
            
            float angle = Mathf.Atan2(inputVector.x, inputVector.z) * Mathf.Rad2Deg;
            float smooth = Mathf.SmoothDampAngle(transform.eulerAngles.y, angle, ref rotAngle, 0.1f);
            
            transform.rotation = Quaternion.Euler(0, smooth, 0);
        }
        else
            animator.SetFloat(vertical, 0);
        
    }

    private void GetInputs()
    {
        movement = movementActionRef.action.ReadValue<Vector2>();
        inputVector = new Vector3(movement.x, 0, movement.y);

        isSprinting = sprintActionRef.action.IsPressed();
    }
    
    private void Melee(InputAction.CallbackContext obj)
    {
        animator.SetTrigger(stabbing);
    }

    private void OnEnable()
    {
        movementActionRef.action.Enable();
        sprintActionRef.action.Enable();
    }

    private void OnDisable()
    {
        movementActionRef.action.Disable();
        sprintActionRef.action.Disable();
    }
}